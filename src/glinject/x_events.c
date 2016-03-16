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
#include "x_events.h"
#include <X11/Xlib.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

GLINJECT_DEFINE_AND_OVERLOAD
(XCheckIfEvent, Bool, (Display* display, XEvent* event, XIfEvent_predicate_type predicate, XPointer arg)) {
	Bool return_val = glinject_real_XCheckIfEvent(display, event, predicate,
			arg);
	if (return_val) {
		glinject_handle_x11_event(event);
	}
	return return_val;
}
GLINJECT_DEFINE_AND_OVERLOAD(XIfEvent, int,
		(Display* display, XEvent* event, XIfEvent_predicate_type predicate, XPointer pointer)) {
	int return_val = glinject_real_XIfEvent(display, event, predicate, pointer);
	glinject_handle_x11_event(event);
	return return_val;
}

GLINJECT_DEFINE_AND_OVERLOAD(XMaskEvent, int,
		(Display* display, long mask, XEvent* event)) {
	int return_val = glinject_real_XMaskEvent(display, mask, event);
	glinject_handle_x11_event(event);
	return return_val;
}

GLINJECT_DEFINE_AND_OVERLOAD(XNextEvent, int,
		(Display* display, XEvent* event)) {
	int return_val = glinject_real_XNextEvent(display, event);
	glinject_handle_x11_event(event);
	return return_val;
}

GLINJECT_DEFINE_AND_OVERLOAD(XWindowEvent, int,
		(Display* display, Window window, long mask, XEvent* event)) {
	int return_val = glinject_real_XWindowEvent(display, window, mask, event);
	glinject_handle_x11_event(event);
	return return_val;
}

void glinject_load_x_event_real_symbols(const char* path) {
	void *handle = dlopen("libX11.so.6", RTLD_LOCAL | RTLD_LAZY);
	if(!handle){
        fprintf(stderr, "Couldn't load X symbols: %s\n", dlerror());
        exit(EXIT_FAILURE);
	}

	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, XIfEvent);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, XCheckIfEvent);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, XMaskEvent);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, XNextEvent);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, XWindowEvent);
}
