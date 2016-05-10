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

// This REMOVES the event from the queue
GLINJECT_DEFINE_AND_OVERLOAD(XNextEvent, int, (Display *dpy, XEvent *event)) {
	int return_val;
	// Consume the event until we reach one that isn't meant for GLXOSD
	do {
		return_val = glinject_real_XNextEvent(dpy, event);
	} while (glinject_handle_x11_event(event));
	return return_val;
}

// This DOES NOT remove the event from the queue
GLINJECT_DEFINE_AND_OVERLOAD(XPeekEvent, int, (Display *dpy, XEvent *event)) {
	int return_val = glinject_real_XPeekEvent(dpy, event);
	while (glinject_handle_x11_event(event)) {
		// If this event is meant for GLXOSD, there is no reason for the host application to know about it
		glinject_real_XNextEvent(dpy, event);
		return_val = glinject_real_XPeekEvent(dpy, event);
	}
	return return_val;
}

// This REMOVES the event from the queue
GLINJECT_DEFINE_AND_OVERLOAD(XWindowEvent, int, (Display *dpy, Window w, long mask, XEvent *event)) {
	int return_val;
	// Consume the event until we reach one that isn't meant for GLXOSD
	do {
		return_val = glinject_real_XWindowEvent(dpy, w, mask, event);
	} while (glinject_handle_x11_event(event));
	return return_val;
}

// This REMOVES the event from the queue
GLINJECT_DEFINE_AND_OVERLOAD(XCheckWindowEvent, Bool, (Display *dpy, Window w, long mask, XEvent *event)) {
	Bool return_val;
	// Consume the event until we reach one that isn't meant for GLXOSD or we run out of events
	do {
		return_val = glinject_real_XCheckWindowEvent(dpy, w, mask, event);
	} while (return_val && glinject_handle_x11_event(event));
	return return_val;
}

// This REMOVES the event from the queue
GLINJECT_DEFINE_AND_OVERLOAD(XMaskEvent, int, (Display *dpy, long mask, XEvent *event)) {
	int return_val;
	// Consume the event until we reach one that isn't meant for GLXOSD
	do {
		return_val = glinject_real_XMaskEvent(dpy, mask, event);
	} while (glinject_handle_x11_event(event));
	return return_val;
}

// This REMOVES the event from the queue
GLINJECT_DEFINE_AND_OVERLOAD(XCheckMaskEvent, Bool, (Display *dpy, long mask, XEvent *event)) {
	Bool return_val;
	// Consume the event until we reach one that isn't meant for GLXOSD or we run out of events
	do {
		return_val = glinject_real_XCheckMaskEvent(dpy, mask, event);
	} while (return_val && glinject_handle_x11_event(event));
	return return_val;
}

// This REMOVES the event from the queue
GLINJECT_DEFINE_AND_OVERLOAD(XCheckTypedEvent, Bool, (Display *dpy, int type, XEvent *event)) {
	Bool return_val;
	// Consume the event until we reach one that isn't meant for GLXOSD or we run out of events
	do {
		return_val = glinject_real_XCheckTypedEvent(dpy, type, event);
	} while (return_val && glinject_handle_x11_event(event));
	return return_val;
}

// This REMOVES the event from the queue
GLINJECT_DEFINE_AND_OVERLOAD(XCheckTypedWindowEvent, Bool, (Display *dpy, Window w, int type, XEvent *event)) {
	Bool return_val;
	// Consume the event until we reach one that isn't meant for GLXOSD or we run out of events
	do {
		return_val = glinject_real_XCheckTypedWindowEvent(dpy, w, type, event);
	} while (return_val && glinject_handle_x11_event(event));
	return return_val;
}

// This REMOVES the event from the queue
GLINJECT_DEFINE_AND_OVERLOAD(XIfEvent, int, (Display *dpy, XEvent *event, Bool(*predicate)(Display *, XEvent *, XPointer), XPointer arg)) {
	int return_val;
	// Consume the event until we reach one that isn't meant for GLXOSD or we run out of events
	do {
		return_val = glinject_real_XIfEvent(dpy, event, predicate, arg);
	} while (glinject_handle_x11_event(event));
	return return_val;
}

// This REMOVES the event from the queue
GLINJECT_DEFINE_AND_OVERLOAD(XCheckIfEvent, Bool, (Display *dpy, XEvent *event, Bool(*predicate)(Display *, XEvent *, XPointer), XPointer arg)) {
	Bool return_val;
	// Consume the event until we reach one that isn't meant for GLXOSD or we run out of events
	do {
		return_val = glinject_real_XCheckIfEvent(dpy, event, predicate, arg);
	} while (return_val && glinject_handle_x11_event(event));
	return return_val;
}

// This DOES NOT remove the event from the queue
GLINJECT_DEFINE_AND_OVERLOAD(XPeekIfEvent, int, (Display * dpy, XEvent * event, Bool(*predicate) (Display *, XEvent *, XPointer), XPointer arg)) {
	int return_val = glinject_real_XPeekIfEvent(dpy, event, predicate, arg);
	while (glinject_handle_x11_event(event)) {
		// If this event is meant for GLXOSD, there is no reason for the host application to know about it
		glinject_real_XIfEvent(dpy, event, predicate, arg);
		return_val = glinject_real_XPeekIfEvent(dpy, event, predicate, arg);
	}
	return return_val;
}

GLINJECT_DEFINE_AND_OVERLOAD(XPending, int, (Display *dpy)) {
	// Consume all events that are to be processed by GLXOSD
	XEvent event;
	while (XCheckIfEvent(dpy, &event, glinject_check_if_event, NULL));
	return glinject_real_XPending(dpy);
}

void glinject_load_x_event_real_symbols(const char* path) {
	void *handle = dlopen("libX11.so.6", RTLD_LOCAL | RTLD_LAZY);
	if (!handle) {
		fprintf(stderr, "Couldn't load X symbols: %s\n", dlerror());
		exit(EXIT_FAILURE);
	}

	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, XPending);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, XNextEvent);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, XPeekEvent);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, XWindowEvent);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, XCheckWindowEvent);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, XMaskEvent);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, XCheckMaskEvent);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, XCheckTypedEvent);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, XCheckTypedWindowEvent);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, XIfEvent);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, XCheckIfEvent);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, XPeekIfEvent);
}
