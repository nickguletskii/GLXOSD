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
require("ffi/freetype-gl")
local ffi = require("ffi")

ffi_types = {}

local types = {
	"FcResult",
	"GLboolean",
	"GLchar",
	"GLfloat",
	"GLint",
	"GLuint",
	"GLsizei",
	"GLenum",
	"markup_t",
	"mat4",
	"vec2",
	"vec4",
	"vertex_t",
	"timespec",
	"int",
	"double"
}
for k, v in ipairs(types) do
	ffi_types[v] = ffi.typeof(v)
	ffi_types[v .. "_ptr"] = ffi.typeof(v .. "*")
	ffi_types[v .. "_ref"] = ffi.typeof(v .. "[1]")
	ffi_types[v .. "_arr"] = ffi.typeof(v .. "[?]")
end

ffi_types["intptr_t"] = ffi.typeof("intptr_t")
ffi_types["char_arr"] = ffi.typeof("char[?]")
ffi_types["const_char_ptr"] = ffi.typeof("const char *")
ffi_types["const_char_ptr_ref"] = ffi.typeof("const char *[1]")
ffi_types["const_int_ref"] = ffi.typeof("const int[1]")
ffi_types["unsigned_int_ref"] = ffi.typeof("unsigned int[1]")
ffi_types["FcChar8_ptr_ref"] = ffi.typeof("FcChar8 *[1]")
ffi_types["const_FcChar8_ptr"] = ffi.typeof("FcChar8 *")
ffi_types["unsigned_long_long"] = ffi.typeof("unsigned long long")
ffi_types["unsigned_long_long_ref"] = ffi.typeof("unsigned long long[1]")

ffi_types["char_array_from_string"] =function (str)
	local out = ffi_types.char_arr(#str+1);
	ffi.copy(out, ffi.cast(ffi_types.const_char_ptr, str), #str)
	out[#str] = 0
	return out
end
