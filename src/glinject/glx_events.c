/*
 * Copyright (C) 2013-2016 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "glinject.h"
#include "glx_events.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>

GLINJECT_DEFINE_REAL_SYMBOL(glXQueryContext, int,
		( Display *dpy, GLXContext ctx, int attribute, int *value ));

GLINJECT_DEFINE_REAL_SYMBOL(glXGetCurrentContext, GLXContext, ())

GLINJECT_DEFINE_AND_OVERLOAD( glXDestroyContext,void,(Display *dpy, GLXContext ctx) ) {
	glinject_init();
	glinject_handle_context_destruction(dpy, ctx);
	glinject_real_glXDestroyContext(dpy, ctx);
}
GLINJECT_DEFINE_AND_OVERLOAD( glXSwapBuffers, void,(Display* dpy, GLXDrawable ctx) ) {
	glinject_init();
	glinject_handle_buffer_swap(dpy, ctx);
	glinject_real_glXSwapBuffers(dpy, ctx);
}

__GLXextFuncPtr glXGetProcAddressARB(const GLubyte *name) {
	glinject_init();
	void* overriddenFunction = glinject_get_function_override((const char *) name);
	if (overriddenFunction != NULL)
		return (__GLXextFuncPtr) overriddenFunction;
	return glinject_real_glXGetProcAddressARB(name);
}
__GLXextFuncPtr glXGetProcAddress(const GLubyte *name) {
	glinject_init();
	void* overriddenFunction = glinject_get_function_override((const char *) name);
	if (overriddenFunction != NULL)
		return (__GLXextFuncPtr) overriddenFunction;
	return glinject_real_glXGetProcAddress(name);
}

void glinject_load_glx_event_real_symbols(const char* path) {
	void *handle = dlopen(path, RTLD_LOCAL | RTLD_LAZY);
	if(!handle){
        fprintf(stderr, "Couldn't load GLX symbols: %s\n", dlerror());
        exit(EXIT_FAILURE);
	}

	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, glXDestroyContext);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, glXGetProcAddressARB);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, glXGetProcAddress);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, glXSwapBuffers);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, glXQueryContext);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, glXGetCurrentContext);
}
