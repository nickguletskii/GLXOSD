/*
 * Copyright (C) 2013-2016 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef GLINJECT_X_EVENTS_H_
#define GLINJECT_X_EVENTS
#include "glinject.h"

typedef Bool (*XIfEvent_predicate_type)(Display* display, XEvent* event,
		XPointer pointer);

GLINJECT_DEFINE_REAL_SYMBOL(XCheckIfEvent, Bool,
		(Display* display, XEvent* event, XIfEvent_predicate_type predicate, XPointer arg));

GLINJECT_DEFINE_REAL_SYMBOL(XIfEvent, int,
		(Display* display, XEvent* event, XIfEvent_predicate_type predicate, XPointer pointer));

GLINJECT_DEFINE_REAL_SYMBOL(XMaskEvent, int,
		(Display* display, long mask, XEvent* event));

GLINJECT_DEFINE_REAL_SYMBOL(XNextEvent, int, (Display* display, XEvent* event));

GLINJECT_DEFINE_REAL_SYMBOL(XWindowEvent, int,
		(Display* display, Window window, long mask, XEvent* event));

void glinject_load_x_event_real_symbols(const char* path);

#endif
