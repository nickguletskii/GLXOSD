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

#define DEFINE_REAL_SYMBOL(name, ret, param)\
		typedef ret (*name##_type) param;\
		name##_type real_##name = nullptr;

#define DEFINE_AND_OVERLOAD_X11_EVENT_PROCESSING_SYMBOL(name, ret, param, param2)\
		DEFINE_REAL_SYMBOL(name, ret, param);\
		extern "C" ret name param {\
			init_gl_frame_hooks();\
			ret returnVal = real_##name param2;\
			if (event->type == KeyPress) {\
				handle_key_event(event);\
			}\
			return returnVal;\
		}

#define LOAD_SYMBOL_USING_DLSYM(x)\
	real_##x = (x##_type) \
	real_dlsym(RTLD_NEXT, #x);\
	assertSymbolLoaded(real_##x, #x);

void init_gl_frame_hooks();
void handle_key_event(XEvent* event);
typedef std::pair<std::string, void*> function_overload_type;

//Real symbol definitions
DEFINE_REAL_SYMBOL(dlopen, void*, (const char *, int));

DEFINE_REAL_SYMBOL(dlsym, void*, (const void *, const char *));

DEFINE_REAL_SYMBOL(dlvsym, void*, (const void *, const char *, const char *));

DEFINE_REAL_SYMBOL(glXDestroyContext, void, (Display *, GLXContext));

DEFINE_REAL_SYMBOL(glXSwapBuffers, void, (Display*, GLXDrawable));

DEFINE_REAL_SYMBOL(glxinjectConstructor, void, ());

DEFINE_REAL_SYMBOL(glXGetProcAddressARB, __GLXextFuncPtr, (const GLubyte*));

DEFINE_AND_OVERLOAD_X11_EVENT_PROCESSING_SYMBOL(XIfEvent, int,
		(Display* display, XEvent* event, XIfEvent_predicate_type predicate, XPointer pointer),
		(display, event, predicate, pointer));

DEFINE_AND_OVERLOAD_X11_EVENT_PROCESSING_SYMBOL(XCheckIfEvent, Bool,
		(Display* display, XEvent* event, XIfEvent_predicate_type predicate, XPointer pointer),
		(display, event, predicate, pointer));

DEFINE_AND_OVERLOAD_X11_EVENT_PROCESSING_SYMBOL(XMaskEvent, int,
		(Display* display, long mask, XEvent* event), (display, mask, event));

DEFINE_AND_OVERLOAD_X11_EVENT_PROCESSING_SYMBOL(XNextEvent, int,
		(Display* display, XEvent* event), (display, event));

DEFINE_AND_OVERLOAD_X11_EVENT_PROCESSING_SYMBOL(XWindowEvent, int,
		(Display* display, Window window, long mask, XEvent* event),
		(display, window, mask, event));

std::map<std::string, void*>* function_overrides = nullptr;
std::map<int, gl_frame_handler>* handlers = nullptr;
int last_id = 0;
bool initialised = false;

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

/*
 * Hooking
 */
void* get_function_override(const char* name) {
	if (function_overrides == nullptr) { //Initialise the overridden functions map
		function_overrides = new std::map<std::string, void*>();
		function_overrides->insert(
				function_overload_type("glXDestroyContext",
						(void*) &glXDestroyContext));
		function_overrides->insert(
				function_overload_type("glXSwapBuffers",
						(void*) &glXSwapBuffers));
		function_overrides->insert(
				function_overload_type("glXGetProcAddressARB",
						(void*) &glXGetProcAddressARB));
	}
	std::map<std::string, void*>::iterator it = function_overrides->find(
			std::string(name));
	if (it == function_overrides->end())
		return nullptr;
	return (*(it)).second;
}
void handle_buffer_swap(Display* dpy, GLXDrawable ctx) {
	if (handlers == nullptr)
		handlers = new std::map<int, gl_frame_handler>();
	for (std::map<int, gl_frame_handler>::iterator i = handlers->begin();
			i != handlers->end(); i++) {
		XEvent event;
		XCheckIfEvent(dpy, &event, (*i).second.event_filter, nullptr);
		(*i).second.handle_buffer_swap(dpy, ctx);
	}
}
void handle_context_destruction(Display* dpy, GLXContext ctx) {
	if (handlers == nullptr)
		handlers = new std::map<int, gl_frame_handler>();
	for (std::map<int, gl_frame_handler>::iterator i = handlers->begin();
			i != handlers->end(); i++)
		(*i).second.handle_context_destruction(dpy, ctx);
}
void handle_key_event(XEvent* event) {
	if (handlers == nullptr)
		handlers = new std::map<int, gl_frame_handler>();
	for (std::map<int, gl_frame_handler>::iterator i = handlers->begin();
			i != handlers->end(); i++)
		(*i).second.handle_keyboard_event(&event->xkey);
}

extern "C" void glXDestroyContext(Display *dpy, GLXContext ctx) {
	init_gl_frame_hooks();
	handle_context_destruction(dpy, ctx);
	real_glXDestroyContext(dpy, ctx);
}
extern "C" void glXSwapBuffers(Display* dpy, GLXDrawable ctx) {
	init_gl_frame_hooks();
	handle_buffer_swap(dpy, ctx);
	real_glXSwapBuffers(dpy, ctx);
}
extern "C" __GLXextFuncPtr glXGetProcAddressARB(const GLubyte *name) {
	init_gl_frame_hooks();
	for (std::map<std::string, void*>::iterator it =
			function_overrides->begin(); it != function_overrides->end(); it++)
		if (strcmp((char*) name, (*it).first.c_str()) == 0)
			return (__GLXextFuncPtr) ((*it).second);

	return real_glXGetProcAddressARB(name);
}

extern "C" void * dlsym(void *handle, const char *name) {
	init_gl_frame_hooks();
	void* overload = get_function_override(name);
	if (overload != nullptr)
		return overload;
	return real_dlsym(handle, name);
}

extern "C" void * dlsvym(void *handle, const char *name, const char *ver) {
	init_gl_frame_hooks();
	void* overload = get_function_override(name);
	if (overload != nullptr)
		return overload;
	return real_dlvsym(handle, name, ver);
}

template<typename T>
void assertSymbolLoaded(T* symbol, std::string name) {
	if (symbol == nullptr) {
		std::cerr << "Couldn't find " << name
				<< "! dlsym returned a nullptr pointer." << std::endl;
		std::cerr << dlerror() << std::endl;
		exit(-1);
	}
}

void init_gl_frame_hooks() {
	if (initialised)
		return;
	initialised = true;
	eh_obj_t libdl;
	if (eh_find_obj(&libdl, "*/libdl.so*")) {
		std::cerr << "Couldn't find libdl!" << std::endl;
		exit(-1);
	}
	if (eh_find_sym(&libdl, "dlsym", (void **) &real_dlsym)) {
		std::cerr << "Couldn't find dlsym in libdl!" << std::endl;
		eh_destroy_obj(&libdl);
		exit(-1);
	}
	if (eh_find_sym(&libdl, "dlvsym", (void **) &real_dlvsym)) {
		std::cerr << "Couldn't find dlvsym in libdl!" << std::endl;
		eh_destroy_obj(&libdl);
		exit(-1);
	}
	eh_destroy_obj(&libdl);

	real_dlopen = (dlopen_type) real_dlsym(RTLD_NEXT, "dlopen");
	if (real_dlopen == nullptr) {
		std::cerr << "Couldn't find dlopen! dlsym returned a nullptr pointer."
				<< std::endl;
		std::cerr << dlerror() << std::endl;
		exit(-1);
	}
	LOAD_SYMBOL_USING_DLSYM(glXDestroyContext);
	LOAD_SYMBOL_USING_DLSYM(glXGetProcAddressARB);
	LOAD_SYMBOL_USING_DLSYM(glXSwapBuffers);
	LOAD_SYMBOL_USING_DLSYM(XIfEvent);
	LOAD_SYMBOL_USING_DLSYM(XCheckIfEvent);
	LOAD_SYMBOL_USING_DLSYM(XMaskEvent);
	LOAD_SYMBOL_USING_DLSYM(XNextEvent);
	LOAD_SYMBOL_USING_DLSYM(XWindowEvent);
}

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

/*
 * Library initialisation/construction hooks
 */
void glinject_construct() {
	executeAll(split(getEnvironment("GLINJECT_CONSTRUCTORS"), ':'));
}
void glinject_destruct() {
	executeAll(split(getEnvironment("GLINJECT_DESTRUCTORS"), ':'));
}
