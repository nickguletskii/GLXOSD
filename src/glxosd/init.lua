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

-- Paths in which to look for modules, relative to glxosdPackageRoot.
local packagePaths = {"/LJIT2Khronos/?.lua",
	"/LJIT2Khronos/Win32/?.lua",
	"/glxosd/?.lua"};

-- Build package.path from packagePaths.
local packagePathsCombined = ""
for k, v in ipairs( packagePaths) do
	packagePathsCombined = packagePathsCombined .. glxosdPackageRoot .. v .. ";"
end
package.path = packagePathsCombined ..package.path

local ffi = require("ffi")

-- GLXOSD's dynamic GL loader
ffi.cdef[[
void glinject_dynamic_gl_initialise(const char* libGL_path);
]]

--Load third part library bindings
require("ffi/fontconfig")
require("ffi/gltypes")
require("gl")
require("glext")
require("ffi/glx")
require("ffi/freetype-gl")
require("ffi/ffi_types")

-- Load OpenGL error handling functions into global scope
require("rendering/gl_error")

--Dynamically load third party libraries
_G["gl"] = ffi.load( "libGL.so.1", false)
_G["glew"] = ffi.load( "libGLEW.so", false)
_G["freetype_gl"] = ffi.load("libglxosd-freetype-gl",false);
_G["fc"] = ffi.load("libfontconfig",false);

--[[
	The resource resolver is used to look for additional resources such as
	shaders. This will probably be moved to a distro-specific config file.
]] 
resource_resolver = function(resource)
	return glxosdPackageRoot .. "/" .. resource;
end

--Load the required OpenGL functions so that freetype-gl can do its job.
freetype_gl.glinject_dynamic_gl_initialise("libGL.so.1");

--[[
	do_when_gl_state_is_normal executes the passed function while the OpenGL
	state is in a "normal" state and then reverts everything to the state
	that was set before GLXOSD was called.
]]
local do_when_gl_state_is_normal = require("rendering/normalise_gl_state")

local text_renderer = require("rendering/text_renderer")

function query_drawable(display, drawable, attribute)
	local res = ffi_types.GLint_ref();
	gl.glXQueryDrawable(display, drawable,attribute, res);
	return res[0];
end

function get_font(name)
	local fontConfig = fc.FcInitLoadConfigAndFonts();
	local pattern = fc.FcNameParse(ffi.cast(ffi_types.const_FcChar8_ptr, name));
	fc.FcConfigSubstitute(fontConfig, pattern, fc.FcMatchPattern);
	fc.FcDefaultSubstitute(pattern);
	local result = ffi_types.FcResult_ref();
	local fontFile;
	local font = fc.FcFontMatch(fontConfig, pattern, result);
	if (font) then
		local file = ffi_types.FcChar8_ptr_ref();
		if (fc.FcPatternGetString(font, FC_FILE, 0, file) == fc.FcResultMatch) then
			fontFile = ffi.string(file[0]);
		end
		fc.FcPatternDestroy(font);
	end
	fc.FcPatternDestroy(pattern);
	fc.FcConfigDestroy(fontConfig);
	fc.FcFini();
	return fontFile;
end

local renderer = nil;
function handle_buffer_swap(display, drawable)
	local width = query_drawable(display, drawable, GLX_WIDTH);
	local height = query_drawable(display, drawable, GLX_HEIGHT);

	local viewport = ffi_types.GLint_arr(4);
	gl.glGetIntegerv(GL_VIEWPORT, viewport);

	check_gl_error();
	do_when_gl_state_is_normal(function()
		if(renderer == nil) then
			renderer = text_renderer.new(get_font("Ubuntu:style=Bold"), 32);
		end
		renderer.setText("hello world: "..width.." "..height.."\nThe quick brown fox jumps over the lazy dog");
		renderer.render(width, height);
	end)

	gl.glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
end
