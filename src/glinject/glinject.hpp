/*
 * Copyright (C) 2013-2014 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef GLINJECT_HPP_
#define GLINJECT_HPP_

#include <GL/glx.h>
#include <X11/Xlib.h>

/*
 * glinject external API
 */
typedef Bool (*XIfEvent_predicate_type)(Display* display, XEvent* event, XPointer pointer);
typedef void (*handle_buffer_swap_type)(Display*, GLXDrawable);
typedef void (*handle_context_destruction_type)(Display*, GLXContext);
typedef void (*handle_keyboard_event_type)(XKeyEvent*);
extern "C" struct gl_frame_handler {
	handle_buffer_swap_type handle_buffer_swap;
	handle_context_destruction_type handle_context_destruction;
	handle_keyboard_event_type handle_keyboard_event;
	XIfEvent_predicate_type event_filter;
};
extern "C" int glinject_add_gl_frame_handler(gl_frame_handler handler);
extern "C" bool glinject_remove_gl_frame_handler(int id);

/*
 * Library initialisation/construction hooks
 */
__attribute__((constructor)) void glinject_construct();
__attribute__((destructor)) void glinject_destruct();

#endif /* GLINJECT_HPP_ */
