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

local function shader_error(handle, msg)
	local infoLogLength = ffi_types.GLint_ref()
	gl.glGetShaderiv(handle, GL_INFO_LOG_LENGTH, infoLogLength)
	local infoLog = ffi_types.GLchar_arr(infoLogLength[0]+1)
	gl.glGetShaderInfoLog(handle, infoLogLength[0], nil, infoLog)
	io.stderr:write(ffi.string(infoLog, infoLogLength[0]))
	io.stderr:write(debug.traceback() .. "\n")
	error(msg)
end

local function create_shader(type, source)
	local src = ffi_types.const_char_ptr_ref()
	src[0] = source
	local handle = gl.glCreateShader(type)
	local lengths = ffi_types.const_int_ref({#source})

	gl.glShaderSource(handle, 1, src, lengths)
	gl.glCompileShader(handle)

	local compile_status = ffi_types.GLint_ref()
	gl.glGetShaderiv(handle, GL_COMPILE_STATUS, compile_status)
	if (compile_status[0] == GL_FALSE) then
		shader_error(handle, "Failed to compile the shader!")
	end

	return handle
end

local function create_shader_program(vertex_shader, fragment_shader)
	local program = gl.glCreateProgram()
	if vertex_shader then
		local vert_shader = create_shader(GL_VERTEX_SHADER, vertex_shader)
		gl.glAttachShader(program, vert_shader)
		gl.glDeleteShader(vert_shader)
	end
	if vertex_shader then
		local frag_shader = create_shader(GL_FRAGMENT_SHADER, fragment_shader)
		gl.glAttachShader(program, frag_shader)
		gl.glDeleteShader(frag_shader)
	end
	gl.glLinkProgram(program)

	local link_status = ffi_types.GLint_ref()
	gl.glGetProgramiv(program, GL_LINK_STATUS, link_status)
	if (link_status[0] == GL_FALSE) then
		shader_error(program, "Failed to link the shader program!")
	end
	return program
end

return {
	create_shader = create_shader,
	create_shader_program =create_shader_program
}
