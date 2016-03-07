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

local ffi = require("ffi")

function new_attrib_state_holder(enableFunc, disableFunc, getFunc)
	local storage = {}
	return {
		set = function (pname, param)
			if storage[pname] == nil then
				storage[pname] = getFunc(pname)
			end
			if param == GL_TRUE and storage[pname] == GL_FALSE then
				enableFunc(pname)
			end
			if param == GL_FALSE and storage[pname] == GL_TRUE then
				disableFunc(pname)
			end
		end,
		reset_all = function ()
			for k, v in pairs(storage) do
				local current = getFunc(k)
				if(current ~= v) then
					if v == GL_TRUE then
						enableFunc(k)
					else
						disableFunc(k)
					end
				end
			end
		end
	}
end
local program = ffi_types.GLint_ref()
local front_face = ffi_types.GLint_ref()
local blend_color = ffi_types.GLfloat_arr(4)
local colour_mask = ffi_types.GLboolean_arr(4)
local blend_src = ffi_types.GLint_ref()
local blend_dst = ffi_types.GLint_ref()
local pixel_unpack_buffer_binding = ffi_types.GLint_ref()
local arra_buffer_binding = ffi_types.GLint_ref()
local active_texture = ffi_types.GLint_ref()
local vertex_array_binding = ffi_types.GLint_ref()
local element_array_binding = ffi_types.GLint_ref()
local draw_framebuffer_binding = ffi_types.GLint_ref()
local read_framebuffer_binding = ffi_types.GLint_ref()
local gl_polygon_mode = ffi_types.GLint_ref()
local unpack_swap_buffers = ffi_types.GLint_ref()
local unpack_lsb_first = ffi_types.GLint_ref()
local unpack_row_length = ffi_types.GLint_ref()
local unpack_image_height = ffi_types.GLint_ref()
local unpack_skip_rows = ffi_types.GLint_ref()
local unpack_skip_pixels = ffi_types.GLint_ref()
local unpack_skip_images = ffi_types.GLint_ref()
local unpack_alignment = ffi_types.GLint_ref()
local texture_binding_2d = ffi_types.GLint_ref()
local sampler_binding = ffi_types.GLint_ref()
function normalise(when_normalised)

	-- BEGIN MEMORISE AND SET MISC
	gl.glGetIntegerv(GL_CURRENT_PROGRAM, program)
	gl.glGetIntegerv(GL_FRONT_FACE, front_face)
	gl.glGetFloatv(GL_BLEND_COLOR, blend_color)
	gl.glGetBooleanv(GL_COLOR_WRITEMASK, colour_mask)
	gl.glGetIntegerv(GL_BLEND_SRC, blend_src)
	gl.glGetIntegerv(GL_BLEND_DST, blend_dst)
	-- END MEMORISE AND SET MISC

	-- BEGIN MEMORISE AND SET ATTRIB
	local attr = new_attrib_state_holder(gl.glEnable, gl.glDisable, gl.glIsEnabled)
	attr.set(GL_BLEND, GL_TRUE)
	attr.set(GL_COLOR_LOGIC_OP, GL_FALSE)
	attr.set(GL_CULL_FACE, GL_FALSE)
	attr.set(GL_DEPTH_CLAMP, GL_FALSE)
	attr.set(GL_DEPTH_TEST, GL_FALSE)
	attr.set(GL_DITHER, GL_FALSE)
	attr.set(GL_FRAMEBUFFER_SRGB, GL_FALSE)
	attr.set(GL_LINE_SMOOTH, GL_FALSE)
	attr.set(GL_MULTISAMPLE, GL_FALSE)
	attr.set(GL_POLYGON_OFFSET_FILL, GL_FALSE)
	attr.set(GL_POLYGON_OFFSET_LINE, GL_FALSE)
	attr.set(GL_POLYGON_OFFSET_POINT, GL_FALSE)
	attr.set(GL_POLYGON_SMOOTH, GL_FALSE)
	attr.set(GL_PRIMITIVE_RESTART, GL_FALSE)
	attr.set(GL_SAMPLE_ALPHA_TO_COVERAGE, GL_FALSE)
	attr.set(GL_SAMPLE_ALPHA_TO_ONE, GL_FALSE)
	attr.set(GL_SAMPLE_COVERAGE, GL_FALSE)
	attr.set(GL_SCISSOR_TEST, GL_FALSE)
	attr.set(GL_STENCIL_TEST, GL_FALSE)
	attr.set(GL_TEXTURE_CUBE_MAP_SEAMLESS, GL_FALSE)
	attr.set(GL_PROGRAM_POINT_SIZE, GL_FALSE)
	-- END MEMORISE AND SET ATTRIB

	-- BEGIN MEMORISE BUFFER STATE
	gl.glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, pixel_unpack_buffer_binding)
	gl.glGetIntegerv(GL_ARRAY_BUFFER_BINDING, arra_buffer_binding)
	gl.glGetIntegerv(GL_ACTIVE_TEXTURE, active_texture)
	gl.glGetIntegerv(GL_VERTEX_ARRAY_BINDING, vertex_array_binding)
	gl.glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, element_array_binding)
	gl.glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, draw_framebuffer_binding)
	gl.glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, read_framebuffer_binding)
	gl.glGetIntegerv(GL_POLYGON_MODE, gl_polygon_mode)
	-- END MEMORISE BUFFER STATE

	-- BEGIN MEMORISE UNPACK STATES
	gl.glGetIntegerv(GL_UNPACK_SWAP_BYTES, unpack_swap_buffers)
	gl.glGetIntegerv(GL_UNPACK_LSB_FIRST, unpack_lsb_first)
	gl.glGetIntegerv(GL_UNPACK_ROW_LENGTH, unpack_row_length)
	gl.glGetIntegerv(GL_UNPACK_IMAGE_HEIGHT, unpack_image_height)
	gl.glGetIntegerv(GL_UNPACK_SKIP_ROWS, unpack_skip_rows)
	gl.glGetIntegerv(GL_UNPACK_SKIP_PIXELS, unpack_skip_pixels)
	gl.glGetIntegerv(GL_UNPACK_SKIP_IMAGES, unpack_skip_images)
	gl.glGetIntegerv(GL_UNPACK_ALIGNMENT, unpack_alignment)
	-- END MEMORISE UNPACK STATES

	-- BEGIN MEMORISE GL_TEXTURE_0
	gl.glActiveTexture(GL_TEXTURE0)
	gl.glGetIntegerv(GL_SAMPLER_BINDING, sampler_binding)
	gl.glGetIntegerv(GL_TEXTURE_BINDING_2D, texture_binding_2d)
	-- END MEMORISE GL_TEXTURE_0

	-- BEGIN NORMALISE
	gl.glUseProgram(0)
	gl.glBlendColor(0, 0, 0, 0)
	gl.glFrontFace(GL_CCW)
	gl.glColorMask(1, 1, 1, 1)
	gl.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
	gl.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0)
	gl.glBindFramebuffer(GL_READ_FRAMEBUFFER, 0)
	gl.glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)
	gl.glActiveTexture(GL_TEXTURE0)
	gl.glBindSampler(0, 0)
	gl.glBindTexture(GL_TEXTURE_2D, 0)
	gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
	gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
	gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
	gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)
	gl.glPixelStorei(GL_UNPACK_SWAP_BYTES, false)
	gl.glPixelStorei(GL_UNPACK_LSB_FIRST, false)
	gl.glPixelStorei(GL_UNPACK_ROW_LENGTH, 0)
	gl.glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, 0)
	gl.glPixelStorei(GL_UNPACK_SKIP_ROWS, 0)
	gl.glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0)
	gl.glPixelStorei(GL_UNPACK_SKIP_IMAGES, 0)
	gl.glPixelStorei(GL_UNPACK_ALIGNMENT, 4)
	gl.glBindBuffer(GL_ARRAY_BUFFER, 0)

	-- END NORMALISE

	local status, err = xpcall(when_normalised, function(x)
		log_error("Coudln't run renderer function. OpenGL state will be restored.")
		return x;
	end)

	-- BEGIN RESET GL_TEXTURE_0
	gl.glActiveTexture(GL_TEXTURE0)
	gl.glBindSampler(0, sampler_binding[0])
	gl.glBindTexture(GL_TEXTURE_2D, texture_binding_2d[0])
	-- END RESET GL_TEXTURE_0

	-- BEGIN RESET UNPACK STATES
	gl.glPixelStorei(GL_UNPACK_SWAP_BYTES, unpack_swap_buffers[0])
	gl.glPixelStorei(GL_UNPACK_LSB_FIRST, unpack_lsb_first[0])
	gl.glPixelStorei(GL_UNPACK_ROW_LENGTH, unpack_row_length[0])
	gl.glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, unpack_image_height[0])
	gl.glPixelStorei(GL_UNPACK_SKIP_ROWS, unpack_skip_rows[0])
	gl.glPixelStorei(GL_UNPACK_SKIP_PIXELS, unpack_skip_pixels[0])
	gl.glPixelStorei(GL_UNPACK_SKIP_IMAGES, unpack_skip_images[0])
	gl.glPixelStorei(GL_UNPACK_ALIGNMENT, unpack_alignment[0])
	-- END RESET UNPACK STATES


	-- BEGIN RESET BUFFER STATE
	gl.glActiveTexture(active_texture[0])
	gl.glBindVertexArray(vertex_array_binding[0])
	gl.glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixel_unpack_buffer_binding[0])
	gl.glBindBuffer(GL_ARRAY_BUFFER, arra_buffer_binding[0])
	gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_binding[0])
	gl.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw_framebuffer_binding[0])
	gl.glBindFramebuffer(GL_READ_FRAMEBUFFER, read_framebuffer_binding[0])
	gl.glPolygonMode(GL_FRONT_AND_BACK, gl_polygon_mode[0])
	-- END RESET BUFFER STATE

	-- BEGIN RESET ATTRIB
	attr.reset_all()
	-- END RESET ATTRIB

	-- BEGIN RESET MISC
	gl.glBlendColor(blend_color[0], blend_color[1], blend_color[2],
		blend_color[3])
	gl.glBlendFunc(blend_src[0], blend_dst[0])
	gl.glColorMask(colour_mask[0], colour_mask[1], colour_mask[2],
		colour_mask[3])
	gl.glFrontFace(front_face[0])
	gl.glUseProgram(program[0])
	-- END RESET MISC

	if not status then
		error(err)
	end
end


return {
	do_when_gl_state_is_normal = normalise
}
