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
function readAll(file)
	print(file)
	local f = io.open(file, "rb")
	local content = f:read("*all")
	f:close()
	return content
end
local vertex_shader = readAll(resource_resolver("freetype-gl-glxosd/freetype-gl/shaders/text.vert"))
local fragment_shader = readAll(resource_resolver("freetype-gl-glxosd/freetype-gl/shaders/text.frag"))

local shader_support = require("rendering/shader_support")

return {
	new = function (fontFile, fontSize)
		local shader = shader_support.create_shader_program(vertex_shader, fragment_shader)
		local buffer1 = freetype_gl.text_buffer_new_with_program(3, shader)
		local buffer2 = freetype_gl.text_buffer_new_with_program(3, shader)

		local model = ffi_types.mat4_ref()
		local view = ffi_types.mat4_ref()
		local projection = ffi_types.mat4_ref()
		freetype_gl.mat4_set_identity(projection)
		freetype_gl.mat4_set_identity(model)
		freetype_gl.mat4_set_identity(view)

		local red = ffi_types.vec4({r =1, g=0, b=0, a=1})
		local black = ffi_types.vec4({r =0, g=0, b=0, a=1})
		local none = ffi_types.vec4({r =1, g=0, b=0, a=0})
		local fontFileC = ffi_types.char_array_from_string(fontFile)

		local normal =  ffi_types.markup_t_ref()
		normal[0].family = fontFileC
		normal[0].size = fontSize
		normal[0].bold = 0
		normal[0].italic = 0
		normal[0].gamma = 2.
		normal[0].foreground_color = red
		normal[0].background_color = none
		normal[0].underline = 1
		normal[0].underline_color = red
		normal[0].overline = 0
		normal[0].overline_color = none
		normal[0].strikethrough = 0
		normal[0].strikethrough_color = none
		normal[0].outline = 0
		normal[0].font = nil

		local outline =  ffi_types.markup_t_ref(normal)
		outline[0].outline = 1
		outline[0].foreground_color = black
		outline[0].font = freetype_gl.texture_font_new_from_file(buffer2[0].manager[0].atlas, fontSize, fontFile)
		outline[0].font[0].outline_type=1
		outline[0].font[0].outline_thickness=1

		return {
			setText = function (text)
				local pen = ffi_types.vec2({{20, 200}})
				freetype_gl.text_buffer_clear( buffer1)
				freetype_gl.text_buffer_clear( buffer2)
				freetype_gl.text_buffer_printf( buffer1, pen,
					normal, text, nil)
				freetype_gl.text_buffer_align( buffer1, pen, freetype_gl.ALIGN_CENTER )

				local pen = ffi_types.vec2({{20, 200}})
				freetype_gl.text_buffer_printf( buffer2, pen,
					outline, text, nil)
				freetype_gl.text_buffer_align( buffer2, pen, freetype_gl.ALIGN_CENTER )
				freetype_gl.texture_atlas_upload( buffer1[0].manager[0].atlas )
				freetype_gl.texture_atlas_upload( buffer2[0].manager[0].atlas )

			end,
			render = function (width, height)
				gl.glViewport(0, 0, width, height)

				gl.glEnable(GL_BLEND)
				gl.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)

				freetype_gl.mat4_set_orthographic(projection, 0, width, 0, height, -1, 1)

				gl.glUseProgram(shader)
				gl.glUniform1i(gl.glGetUniformLocation(shader, "texture"),
					0)
				gl.glUniformMatrix4fv(gl.glGetUniformLocation(shader, "model"),
					1, 0, model[0].data)
				gl.glUniformMatrix4fv(gl.glGetUniformLocation(shader, "view"),
					1, 0, view[0].data)
				gl.glUniformMatrix4fv(gl.glGetUniformLocation(shader, "projection"),
					1, 0, projection[0].data)

				freetype_gl.text_buffer_render( buffer1 )
				freetype_gl.text_buffer_render( buffer2 )

				check_gl_error()
			end
		}
	end
}
