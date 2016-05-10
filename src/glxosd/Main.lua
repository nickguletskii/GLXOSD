--[[
Copyright (C) 2016 Nick Guletskii

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
]]
require("ffi/init")

-- Load OpenGL error handling functions into global scope
require("rendering/gl_error_handling")

--[[
	The resource resolver is used to look for additional resources such as
	shaders. This will probably be moved to a distro-specific config file.
]] 
resource_resolver = function(resource)
	return glxosdPackageRoot .. "/" .. resource
end

--[[
	do_when_gl_state_is_normal executes the passed function while the OpenGL
	state is in a "normal" state and then reverts everything to the state
	that was set before GLXOSD was called.
]]
local normalise = require("rendering/normalise_gl_state")
local ffi = require("ffi")

local Context = require("Context")

local function identifyGLXContext(glXContext)
	return tostring(ffi.cast(ffi_types.unsigned_long_long,(ffi.cast(ffi_types.intptr_t, glXContext))))
end

local _contexts = {}
local function get_context(display, drawable, glContext)
	glContext = identifyGLXContext(glContext);
	if _contexts[glContext] == nil then
		_contexts[glContext] = {}
	end
	if(_contexts[glContext][drawable] == nil) then
		local context = Context.new()
		local glx_info = {
			width_ref = ffi_types.GLuint_ref(),
			height_ref = ffi_types.GLuint_ref(),
			drawable = drawable,
			display = display
		}
		function glx_info:update()
			gl.glXQueryDrawable(
				self.display,
				self.drawable,
				GLX_WIDTH,
				self.width_ref)
			gl.glXQueryDrawable(
				self.display,
				self.drawable,
				GLX_HEIGHT,
				self.height_ref)
		end
		function glx_info:width()
			if self.width_ref[0] == 0 then
				glx_info:update()
			end
			if self.width_ref[0] == 0 then
				return 1
			end
			return self.width_ref[0]
		end
		function glx_info:height()
			if self.height_ref[0] == 0 then
				glx_info:update()
			end
			if self.height_ref[0] == 0 then
				return 1
			end
			return self.height_ref[0]
		end
		function glx_info:invalidate()
			self.width_ref[0] = 0
			self.height_ref[0] = 0
		end
		context.glx_info = glx_info

		_contexts[glContext][drawable] = context
	end
	return _contexts[glContext][drawable]
end

local function for_each_context(func)
	for _, drawables in pairs(_contexts) do
		for _, context in pairs(drawables) do
			func(context)
		end
	end
end

local function has_key_combo(key, modifiers)
	for _, drawables in pairs(_contexts) do
		for _, context in pairs(drawables) do
			if context:consumes_keyboard_combo(key, modifiers) then
				return true
			end
		end
	end
	return false
end

function handle_buffer_swap(display, drawable)
	if glxosd_configuration_error then
		return
	end
	local glXContextHandle = identifyGLXContext(gl.glXGetCurrentContext())

	local context = get_context(display, drawable, glXContextHandle)
	context:begin_frame()

	if context:should_render() then
		local width = context.glx_info:width()
		local height = context.glx_info:height()

		local viewport = ffi_types.GLint_arr(4)
		gl.glGetIntegerv(GL_VIEWPORT, viewport)

		normalise.do_when_gl_state_is_normal(function()
			context:render(width, height)
		end, context)

		gl.glViewport(
			ffi.cast(ffi_types.GLint,viewport[0]),
			ffi.cast(ffi_types.GLint,viewport[1]),
			ffi.cast(ffi_types.GLuint,viewport[2]),
			ffi.cast(ffi_types.GLuint,viewport[3])
		)
	end

	context:end_frame()
end

function handle_drawable_destruction(display, drawable)
	if glxosd_configuration_error then
		return
	end

	for glxContext, contexts in pairs(_contexts) do
		if contexts[drawable] then
			contexts[drawable]:destroy()
			contexts[drawable] = nil
		end
	end
	collectgarbage()
end

function handle_context_destruction(display, glXContext)
	if glxosd_configuration_error then
		return
	end

	local glXContextHandle = identifyGLXContext(glXContext)

	if not _contexts[glXContextHandle] then
		return
	end
	for _, context in pairs(_contexts[glXContextHandle]) do
		context:destroy()
	end
	_contexts[glXContextHandle] = nil
	collectgarbage()
end

function should_consume_configure_notify_event()
	if glxosd_configuration_error then
		return false
	end
	for_each_context(function(context)
		context.glx_info:invalidate()
	end)
	return false
end

function should_consume_key_press_event(key, modifiers)
	if glxosd_configuration_error then
		return false
	end
	return has_key_combo(key, modifiers)
end

function key_press_event(key, modifiers)
	if glxosd_configuration_error then
		return
	end
	for_each_context(function(context)
		context:handle_key_combo(key, modifiers)
	end)
	return has_key_combo(key, modifiers)
end

function configure_notify_event(event)
	if glxosd_configuration_error then
		return
	end
	for_each_context(function(context)
		context.glx_info:invalidate()
	end)
end
