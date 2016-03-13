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
require("ffi/clock")
require("ffi/ffitypes")
--Dynamically load third party libraries
_G["gl"] = ffi.load( "libGL.so.1", false)
_G["glew"] = ffi.load( "libGLEW.so.1.10", false)
_G["freetype_gl"] = ffi.load("libglxosd-freetype-gl",false);
_G["fc"] = ffi.load("libfontconfig.so.1",false);

--Load the required OpenGL functions so that freetype-gl can do its job.
freetype_gl.glinject_dynamic_gl_initialise("libGL.so.1");
