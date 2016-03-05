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

ffi_types = {}

local types = {
	"FcResult",
	"GLboolean",
	"GLchar",
	"GLfloat",
	"GLint",
	"GLuint",
	"markup_t",
	"mat4",
	"vec2",
	"vec4",
	"vertex_t"
}
for k, v in ipairs(types) do
	ffi_types[v] = ffi.typeof(v)
	ffi_types[v .. "_ptr"] = ffi.typeof(v .. "*")
	ffi_types[v .. "_ref"] = ffi.typeof(v .. "[1]")
	ffi_types[v .. "_arr"] = ffi.typeof(v .. "[?]")
end

ffi_types["const_char_ptr_ref"] = ffi.typeof("const char *[1]")
ffi_types["const_int_ref"] = ffi.typeof("const int[1]")
ffi_types["FcChar8_ptr_ref"] = ffi.typeof("FcChar8 *[1]")
ffi_types["const_FcChar8_ptr"] = ffi.typeof("FcChar8 *")
ffi_types["char_array_from_string"] =function (str)
	local out = ffi.new("char[?]", #str+1);
	ffi.copy(out,str)
	out[#str] = 0
	return out
end
