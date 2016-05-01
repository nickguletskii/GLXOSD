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

check_gl_error = function ()
	local flag = false
	while true do
		local err = gl.glGetError()
		if (err == GL_NO_ERROR) then
			if flag then
				error("An OpenGL error occurred!")
			end
			return
		end

		local map = {}
		map[GL_INVALID_OPERATION] = "INVALID_OPERATION"
		map[GL_INVALID_ENUM] = "INVALID_ENUM"
		map[GL_INVALID_VALUE] = "INVALID_VALUE"
		map[GL_OUT_OF_MEMORY] = "OUT_OF_MEMORY"
		map[GL_INVALID_FRAMEBUFFER_OPERATION] = "INVALID_FRAMEBUFFER_OPERATION"

		log_error("OpenGL error: " .. map[err])
		flag = true
	end
end
skip_gl_error = function ()
	while true do
		if (gl.glGetError() == GL_NO_ERROR) then
			return
		end
	end
end
