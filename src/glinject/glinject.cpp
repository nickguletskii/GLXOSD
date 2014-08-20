/*
 * Copyright (C) 2013-2014 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "glinject.hpp"
#include "elfhacks.hpp"
#include <dlfcn.h>
#include <GL/gl.h>
#include <stddef.h>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

void init_gl_frame_hooks();
void handle_key_event(XEvent* event);
typedef std::pair<std::string, void*> function_override_type;

std::map<std::string, void*>* function_overrides = nullptr;
std::map<int, gl_frame_handler>* handlers = nullptr;
int last_id = 0;
bool initialised = false;
bool gl_locked = false;
int gl_calls_in_progress = 0;
PTHREADS_MUTEX(gl_locked_mutex);
PTHREADS_COND(gl_locked_condition);
PTHREADS_MUTEX(gl_calls_in_progress_mutex);
PTHREADS_COND(gl_calls_in_progress_condition);

/*
 *  This is ridiculous. RAII doesn't replace the finally block.
 *  Why do I have to write a class just for this?
 */
class globally_synchronized_gl_call_raii_helper {
public:
	globally_synchronized_gl_call_raii_helper() {
		Lock gl_lock_lock(&gl_locked_mutex);
		if (gl_locked)
			pthread_cond_wait(&gl_locked_condition, &gl_locked_mutex);
		Lock gl_call_lock1(&gl_calls_in_progress_mutex);
		gl_lock_lock.unlock();
		gl_calls_in_progress++;
		gl_call_lock1.unlock();
	}
	~globally_synchronized_gl_call_raii_helper() {
		Lock gl_call_lock2(&gl_calls_in_progress_mutex);\
		gl_calls_in_progress--;\
		if (gl_calls_in_progress == 0)
			pthread_cond_broadcast(&gl_calls_in_progress_condition);\
		gl_call_lock2.unlock();\
	}
};

/*
 * Symbol definitions
 */

#define DEFINE_REAL_SYMBOL(name, ret, param)\
		typedef ret (*glinject_##name##_type) param;\
		glinject_##name##_type glinject_real_##name;

DEFINE_REAL_SYMBOL(dlopen, void*, (const char *, int));

DEFINE_REAL_SYMBOL(dlsym, void*, (const void *, const char *));

DEFINE_REAL_SYMBOL(dlvsym, void*, (const void *, const char *, const char *));

DEFINE_REAL_SYMBOL(glXDestroyContext, void, (Display *, GLXContext));

DEFINE_REAL_SYMBOL(glXSwapBuffers, void, (Display*, GLXDrawable));

DEFINE_REAL_SYMBOL(glXGetProcAddressARB, __GLXextFuncPtr, (const GLubyte*));

DEFINE_REAL_SYMBOL(glXGetProcAddress, __GLXextFuncPtr, (const GLubyte*));

#define DEFINE_AND_OVERLOAD_X11_EVENT_PROCESSING_SYMBOL(name, ret, param, param2)\
		DEFINE_REAL_SYMBOL(name, ret, param);\
		extern "C" ret name param {\
			init_gl_frame_hooks();\
			ret returnVal = glinject_real_##name param2;\
			if (event->type == KeyPress) {\
				handle_key_event(event);\
			}\
			return returnVal;\
		}

#define SYNCHRONIZE_FUNCTION_VOID_RET(name, arg, call)\
		PTHREADS_MUTEX(name##mutex);\
		extern "C" void name arg {\
			glinject_real_##name call;\
		}
#define GLOBALLY_SYNCHRONIZE_FUNCTION_VOID_RET(name, arg, call)\
		extern "C" void name arg {\
			globally_synchronized_gl_call_raii_helper gl_call_raii_helper;\
			glinject_real_##name call;\
		}
#define GLOBALLY_SYNCHRONIZE_FUNCTION(name, ret, arg, call)\
		extern "C" ret name arg {\
			globally_synchronized_gl_call_raii_helper gl_call_raii_helper;\
			return glinject_real_##name call;\
		}

#define LOAD_SYMBOL_USING_DLSYM(x)\
	glinject_real_##x = (glinject_##x##_type) \
	glinject_real_dlsym(RTLD_NEXT, #x);\
	assertSymbolLoaded(glinject_real_##x, #x);

#define LOAD_SYMBOL_USING_GLGETPROCADDR(x)\
	glinject_real_##x = (glinject_##x##_type) \
	glinject_real_glXGetProcAddress((const GLubyte*)#x);

#include "overrides.hpp"
#include "syncopengl.hpp"

#undef SYNCHRONIZE_FUNCTION_VOID_RET

#undef SYNCHRONIZE_FUNCTION

#undef GLOBALLY_SYNCHRONIZE_FUNCTION_VOID_RET

#undef GLOBALLY_SYNCHRONIZE_FUNCTION

#undef DEFINE_AND_OVERLOAD_X11_EVENT_PROCESSING_SYMBOL

/*
 * Util functions
 */

static std::vector<std::string> split(std::string str, char separator) {
	std::istringstream ss(str);
	std::string token;
	std::vector<std::string> result;
	while (std::getline(ss, token, separator)) {
		if (token.empty()) {
			continue;
		}
		result.push_back(token);
	}
	return result;
}

static std::string getEnvironment(const std::string & var) {
	const char * val = getenv(var.c_str());
	return (val == nullptr) ? "" : val;
}

template<typename T>
static void assertSymbolLoaded(T* symbol, std::string name) {
	if (symbol == nullptr) {
		std::cerr << "Couldn't find " << name
				<< "! dlsym returned a nullptr pointer." << std::endl;
		std::cerr << dlerror() << std::endl;
		exit(-1);
	}
}
/*
 * Tools to aid initialisation of libraries utilising glinject
 */
template<typename ... Args>
static void executeAll(std::vector<std::string> functionNames, Args ... args) {
	for (size_t i = 0; i < functionNames.size(); i++) {
		void (*constructor)(Args...) =
		(void(*)(Args...)) dlsym(RTLD_NEXT,
				functionNames[i].c_str());
		if (constructor == nullptr) {
			std::cerr << "Couldn't find function " << functionNames[i]
					<< "! dlsym returned a nullptr pointer." << std::endl;
			std::cerr << dlerror() << std::endl;
			exit(-1);
		}
		constructor(args...);
	}
}

/*
 * Initialisation of the overriden functions list
 */

//Redefine definition macros to reuse existing definitions
#define DEFINE_AND_OVERLOAD_X11_EVENT_PROCESSING_SYMBOL(name, ret, param, param2)\
OVERRIDE(name)

#define SYNCHRONIZE_FUNCTION_VOID_RET(name, arg, call)\
OVERRIDE(name)

#define SYNCHRONIZE_FUNCTION(name, arg, call)\
OVERRIDE(name)

#define GLOBALLY_SYNCHRONIZE_FUNCTION_VOID_RET(name, arg, call)\
OVERRIDE(name)

#define GLOBALLY_SYNCHRONIZE_FUNCTION(name, ret, arg, call)\
OVERRIDE(name)

#define OVERRIDE(name)\
		function_overrides->insert(function_override_type(#name, (void*)&name));

std::map<std::string, void*>* get_function_overrides() {

	if (function_overrides == nullptr) {
		function_overrides = new std::map<std::string, void*>();

		OVERRIDE(glXSwapBuffers)

		OVERRIDE(glXDestroyContext)

		OVERRIDE(glXGetProcAddressARB)

		OVERRIDE(glXGetProcAddress)

		OVERRIDE(dlsym)

		OVERRIDE(dlvsym)
// Since we redefined the macros, we can just include the overrides file here...
#include "overrides.hpp"
#include "syncopengl.hpp"
	}
	return function_overrides;
}

#undef SYNCHRONIZE_FUNCTION_VOID_RET

#undef SYNCHRONIZE_FUNCTION

#undef GLOBALLY_SYNCHRONIZE_FUNCTION_VOID_RET

#undef GLOBALLY_SYNCHRONIZE_FUNCTION

#undef DEFINE_AND_OVERLOAD_X11_EVENT_PROCESSING_SYMBOL

void* get_function_override(const char* name) {
	std::map<std::string, void*>::iterator it = get_function_overrides()->find(
			std::string(name));
	if (it == get_function_overrides()->end())
		return nullptr;
	return it->second;
}

/*
 * Handlers
 */
void handle_buffer_swap(Display* dpy, GLXDrawable ctx) {
	if (handlers == nullptr)
		handlers = new std::map<int, gl_frame_handler>();
	for (std::map<int, gl_frame_handler>::iterator i = handlers->begin();
			i != handlers->end(); i++) {
		XEvent event;
		XCheckIfEvent(dpy, &event, i->second.event_filter, nullptr);
		i->second.handle_buffer_swap(dpy, ctx);
	}
}
void handle_context_destruction(Display* dpy, GLXContext ctx) {
	if (handlers == nullptr)
		handlers = new std::map<int, gl_frame_handler>();
	for (std::map<int, gl_frame_handler>::iterator i = handlers->begin();
			i != handlers->end(); i++)
		i->second.handle_context_destruction(dpy, ctx);
}
void handle_key_event(XEvent* event) {
	if (handlers == nullptr)
		handlers = new std::map<int, gl_frame_handler>();
	for (std::map<int, gl_frame_handler>::iterator i = handlers->begin();
			i != handlers->end(); i++)
		i->second.handle_keyboard_event(&event->xkey);
}

/*
 * Function overrides
 */

extern "C" void glXDestroyContext(Display *dpy, GLXContext ctx) {
	init_gl_frame_hooks();
	handle_context_destruction(dpy, ctx);
	glinject_real_glXDestroyContext(dpy, ctx);
}
extern "C" void glXSwapBuffers(Display* dpy, GLXDrawable ctx) {
	init_gl_frame_hooks();
	handle_buffer_swap(dpy, ctx);
	glinject_real_glXSwapBuffers(dpy, ctx);
}
extern "C" __GLXextFuncPtr glXGetProcAddressARB(const GLubyte *name) {
	init_gl_frame_hooks();
	void* overriddenFunction = get_function_override((const char *) name);
	if (overriddenFunction != nullptr)
		return (__GLXextFuncPtr) overriddenFunction;
	return glinject_real_glXGetProcAddressARB(name);
}
extern "C" __GLXextFuncPtr glXGetProcAddress(const GLubyte *name) {
	init_gl_frame_hooks();
	void* overriddenFunction = get_function_override((const char *) name);
	if (overriddenFunction != nullptr)
		return (__GLXextFuncPtr) overriddenFunction;
	return glinject_real_glXGetProcAddress(name);
}

extern "C" void * dlsym(void *handle, const char *name) {
	init_gl_frame_hooks();
	void* overload = get_function_override(name);
	if (overload != nullptr)
		return overload;
	return glinject_real_dlsym(handle, name);
}

extern "C" void * dlsvym(void *handle, const char *name, const char *ver) {
	init_gl_frame_hooks();
	void* overload = get_function_override(name);
	if (overload != nullptr)
		return overload;
	return glinject_real_dlvsym(handle, name, ver);
}

gl_function_provider_type get_gl_function_provider() {
	init_gl_frame_hooks();
	return glinject_real_glXGetProcAddress;
}

void init_gl_frame_hooks() {
	if (initialised)
		return;
	initialised = true;

	/*
	 * Load dlsym and dlvsym using libelfhacks because if we were to use dlsym
	 * to load dlsym, we would get our overridden dlsym, which would result in
	 * recursion.
	 * */

	eh_obj_t libdl;
	if (eh_find_obj(&libdl, "*/libdl.so*")) {
		std::cerr << "Couldn't find libdl!" << std::endl;
		exit(-1);
	}
	if (eh_find_sym(&libdl, "dlsym", (void **) &glinject_real_dlsym)) {
		std::cerr << "Couldn't find dlsym in libdl!" << std::endl;
		eh_destroy_obj(&libdl);
		exit(-1);
	}
	if (eh_find_sym(&libdl, "dlvsym", (void **) &glinject_real_dlvsym)) {
		std::cerr << "Couldn't find dlvsym in libdl!" << std::endl;
		eh_destroy_obj(&libdl);
		exit(-1);
	}
	eh_destroy_obj(&libdl);

	glinject_real_dlopen = (glinject_dlopen_type) glinject_real_dlsym(RTLD_NEXT,
			"dlopen");
	if (glinject_real_dlopen == nullptr) {
		std::cerr << "Couldn't find dlopen! dlsym returned a nullptr pointer."
				<< std::endl;
		std::cerr << dlerror() << std::endl;
		exit(-1);
	}
	LOAD_SYMBOL_USING_DLSYM(glXDestroyContext);
	LOAD_SYMBOL_USING_DLSYM(glXGetProcAddressARB);
	LOAD_SYMBOL_USING_DLSYM(glXGetProcAddress);
	LOAD_SYMBOL_USING_DLSYM(glXSwapBuffers);
	LOAD_SYMBOL_USING_DLSYM(XIfEvent);
	LOAD_SYMBOL_USING_DLSYM(XCheckIfEvent);
	LOAD_SYMBOL_USING_DLSYM(XMaskEvent);
	LOAD_SYMBOL_USING_DLSYM(XNextEvent);
	LOAD_SYMBOL_USING_DLSYM(XWindowEvent);
}

/*
 * API
 */

int glinject_add_gl_frame_handler(gl_frame_handler handler) {
	if (handlers == nullptr)
		handlers = new std::map<int, gl_frame_handler>();
	handlers->insert(std::pair<int, gl_frame_handler>(++last_id, handler));
	return handlers->size() - 1;
}

bool glinject_remove_gl_frame_handler(int id) {
	std::map<int, gl_frame_handler>::iterator it = handlers->find(id);
	if (it == handlers->end())
		return false;
	handlers->erase(it);
	return true;
}

extern "C" void glinject_lock_gl() {
	Lock gl_call_lock(&gl_calls_in_progress_mutex);
	if (gl_calls_in_progress > 0)
		pthread_cond_wait(&gl_calls_in_progress_condition,
				&gl_calls_in_progress_mutex);
	Lock lock(&gl_locked_mutex);
	gl_locked = true;
}
extern "C" void glinject_unlock_gl() {
	Lock lock(&gl_locked_mutex);
	gl_locked = false;
	pthread_cond_broadcast(&gl_locked_condition);
}

/*
 * Library initialisation/construction hooks
 */
void glinject_construct() {
	executeAll(split(getEnvironment("GLINJECT_CONSTRUCTORS"), ':'));
}
void glinject_destruct() {
	executeAll(split(getEnvironment("GLINJECT_DESTRUCTORS"), ':'));
}
