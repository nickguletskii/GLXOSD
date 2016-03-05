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
	
GLX_SWAP_INTERVAL_EXT = 0x20F1
GLX_WIDTH = 0x801D
GLX_HEIGHT = 0x801E
	
ffi.cdef[[

extern void (*glXGetProcAddress(const GLubyte *procname))( void );


struct _XDisplay;

typedef struct __GLXcontextRec *GLXContext;
typedef unsigned long XID;
typedef struct _XDisplay Display;
typedef XID GLXDrawable;

void glXQueryDrawable(Display *dpy, GLXDrawable draw, int attribute,
                              unsigned int *value );
GLXContext glXGetCurrentContext( void );
]]
return {}