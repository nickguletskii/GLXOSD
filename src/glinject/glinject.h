/*
 * Copyright (C) 2013-2016 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef GLINJECT_H_
#define GLINJECT_H_

#include <stdbool.h>
#include <GL/glx.h>
#include <X11/Xlib.h>

/*
 * Helper macros
 */
#define GLINJECT_DEFINE_REAL_SYMBOL(name, ret, param)\
		typedef ret (*glinject_##name##_type) param;\
		glinject_##name##_type glinject_real_##name;

#define GLINJECT_DEFINE_AND_OVERLOAD(name, ret, param)\
		GLINJECT_DEFINE_REAL_SYMBOL(name, ret, param);\
		ret name param

#define GLINJECT_LOAD_SYMBOL_USING_DLSYM(lib, x)\
	glinject_real_##x = (glinject_##x##_type) glinject_real_dlsym(lib, #x);\
	glinject_assert_symbol_loaded(glinject_real_##x, #x);

#define GLINJECT_LOAD_SYMBOL_USING_GLGETPROCADDR(x)\
	glinject_real_##x = (glinject_##x##_type) \
	glinject_real_glXGetProcAddress((const GLubyte*)#x);

/*
 * Type definitions
 */
typedef __GLXextFuncPtr (*gl_function_provider_type)(const GLubyte* name);

/*
 * Real symbol definitions
 */
GLINJECT_DEFINE_REAL_SYMBOL(dlopen, void*, (const char *, int));

GLINJECT_DEFINE_REAL_SYMBOL(dlsym, void*, (const void *, const char *));

GLINJECT_DEFINE_REAL_SYMBOL(dlvsym, void*,
		(const void *, const char *, const char *));

/*
 * Function definitions
 */
void glinject_init();
void glinject_assert_symbol_loaded(void* symbol, const char* name);

Bool glinject_check_if_event(Display* display, XEvent* event, XPointer pointer);
bool glinject_handle_x11_event(XEvent* event);
void glinject_handle_buffer_swap(Display* dpy, GLXDrawable drawable);
void glinject_handle_drawable_destruction(Display* dpy, GLXDrawable drawable);
void glinject_handle_context_destruction(Display* dpy, GLXContext context);

gl_function_provider_type glinject_get_real_glXGetProcAddress();
void* glinject_get_function_override(const char* name);

/*
 * Library initialisation/construction hooks
 */
__attribute__((constructor)) void glinject_construct();

#endif /* GLINJECT_H_ */
