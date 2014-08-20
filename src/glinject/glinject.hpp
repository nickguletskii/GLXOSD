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

#include "lock.hpp"
#include <GL/glx.h>
#include <X11/Xlib.h>

typedef __GLXextFuncPtr (*gl_function_provider_type)(const GLubyte* name);
gl_function_provider_type get_gl_function_provider();

/*
 * Utility macro to mask glinject_real_gl
 */
#define rgl(x) glinject_real_gl##x

/*
 * Real symbol definitions
 */
#define DEFINE_REAL_GL_FUNC(name, returns, arg)\
		typedef returns ( * glinject_##name##_type) arg;\
		const glinject_##name##_type glinject_real_##name __attribute__((unused))= \
		(glinject_##name##_type) get_gl_function_provider()((const GLubyte *)#name);

#define GLOBALLY_SYNCHRONIZE_FUNCTION_VOID_RET(name, arg, call)\
		DEFINE_REAL_GL_FUNC(name, void, arg)

#define GLOBALLY_SYNCHRONIZE_FUNCTION(name, ret, arg, call)\
		DEFINE_REAL_GL_FUNC(name, ret, arg)

#define SYNCHRONIZE_FUNCTION_VOID_RET(name, arg, call)\
		DEFINE_REAL_GL_FUNC(name, void, arg)

#define SYNCHRONIZE_FUNCTION(name, ret, arg, call)\
		DEFINE_REAL_GL_FUNC(name, ret, arg)

#include "syncopengl.hpp"

#undef GLOBALLY_SYNCHRONIZE_FUNCTION

#undef GLOBALLY_SYNCHRONIZE_FUNCTION_VOID_RET

#undef SYNCHRONIZE_FUNCTION

#undef SYNCHRONIZE_FUNCTION_VOID_RET

/*
 * glinject external API
 */
typedef Bool (*XIfEvent_predicate_type)(Display* display, XEvent* event,
		XPointer pointer);
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
extern "C" void glinject_lock_gl();
extern "C" void glinject_unlock_gl();

extern "C" pthread_mutex_t GLINJECT_GLOBAL_MUTEX;

/*
 * Library initialisation/construction hooks
 */
__attribute__((constructor)) void glinject_construct();
__attribute__((destructor)) void glinject_destruct();

#endif /* GLINJECT_HPP_ */
