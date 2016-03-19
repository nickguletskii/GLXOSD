/*
 * Copyright (C) 2013-2016 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#define _GNU_SOURCE
#include "glinject.h"
#include "glx_events.h"
#include "x_events.h"
#include "luajit_dynamic.h"
#include "elfhacks.hpp"
#include <dlfcn.h>
#include <GL/gl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define lua_close glinject_real_lua_close
#define lua_createtable glinject_real_lua_createtable
#define lua_isstring glinject_real_lua_isstring
#define lua_pcall glinject_real_lua_pcall
#define lua_pushboolean glinject_real_lua_pushboolean
#define lua_pushlightuserdata glinject_real_lua_pushlightuserdata
#define lua_pushnumber glinject_real_lua_pushnumber
#define lua_pushstring glinject_real_lua_pushstring
#define lua_setfield glinject_real_lua_setfield
#define lua_settop glinject_real_lua_settop
#define lua_toboolean glinject_real_lua_toboolean
#define lua_tolstring glinject_real_lua_tolstring
#define luaL_loadfile glinject_real_luaL_loadfile
#define luaL_newstate glinject_real_luaL_newstate
#define luaL_openlibs glinject_real_luaL_openlibs
#define lua_getfield glinject_real_lua_getfield
#define lua_type glinject_real_lua_type

// Mutex used for synchronising Lua API calls.
pthread_mutex_t glinject_mutex = PTHREAD_MUTEX_INITIALIZER;

static bool initialised = false;
static lua_State *L = 0;

/*
 * ============================================================================
 * Event handling
 * ============================================================================
 */

/*
 * Pushes a table of modifiers onto the Lua stack.
 * {
 * 	shift=true,
 * 	caps=true,
 * 	control=true,
 * 	alt=true
 * }
 */
void glinject_lua_pushkeyeventmodifiers(lua_State* L, XKeyEvent *event) {
	lua_createtable(L, 0, 4);
	lua_pushboolean(L, (event->state & ShiftMask) != 0);
	lua_setfield(L, -2, "shift");
	lua_pushboolean(L, (event->state & LockMask) != 0);
	lua_setfield(L, -2, "caps");
	lua_pushboolean(L, (event->state & ControlMask) != 0);
	lua_setfield(L, -2, "control");
	lua_pushboolean(L, (event->state & Mod1Mask) != 0);
	lua_setfield(L, -2, "alt");
}

/*
 * Handles X11 keyboard events.
 */
void glinject_handle_x11_key_event(XKeyEvent* event) {
	KeySym ks = XLookupKeysym(event, 0);

	lua_getglobal(L, "key_press_event"); /* function to be called */
	if (!lua_isfunction(L, -1)) {
		fprintf(stderr, "key_press is not a function!\n");
		return;
	}
	lua_pushstring(L, XKeysymToString(ks)); // Push the name of the key as the first parameter
	glinject_lua_pushkeyeventmodifiers(L, event); // Push a table of modifiers as the second parameter

	if (lua_pcall(L, 2, 0, 0) != 0) {
		fprintf(stderr, "error running function: %s\n", lua_tostring(L, -1));
	}
}
/*
 * Handles X11 ConfigureNotify events. For instance, this is triggered
 * when a window is resized.
 */
void glinject_handle_x11_configure_notify_event(XEvent* event) {
	lua_getglobal(L, "configure_notify_event"); /* function to be called */
	if (!lua_isfunction(L, -1)) {
		fprintf(stderr, "configure_notify_event is not a function!\n");
		return;
	}
	if (lua_pcall(L, 0, 0, 0) != 0) {
		fprintf(stderr, "error running function: %s\n", lua_tostring(L, -1));
	}
}

/*
 * Predicate for XCheckIfEvent.
 */
Bool glinject_check_if_event(XEvent* event) {
	pthread_mutex_lock(&glinject_mutex);
	if (event->type == ConfigureNotify) {
		lua_getglobal(L, "should_consume_configure_notify_event"); /* function to be called */
		if (!lua_isfunction(L, -1)) {
			fprintf(stderr,
					"should_consume_configure_notify_event is not a function!\n");
			return False;
		}
		glinject_lua_pushkeyeventmodifiers(L, event);
		if (lua_pcall(L, 1, 1, 0) != 0) {
			fprintf(stderr, "error running function: %s\n",
					lua_tostring(L, -1));
		}
		if (!lua_isboolean(L, -1)) {
			fprintf(stderr,
					"should_consume_configure_notify_event must return a boolean\n");
		}
		Bool res = lua_toboolean(L, -1);
		lua_pop(L, 1);
		return res;
	}
	if (event->type == KeyPress) {
		lua_getglobal(L, "should_consume_key_press_event"); /* function to be called */
		if (!lua_isfunction(L, -1)) {
			fprintf(stderr,
					"should_consume_key_press_event is not a function!\n");
			return False;
		}

		KeySym ks = XLookupKeysym((XKeyEvent*) event, 0);

		lua_pushstring(L, XKeysymToString(ks));
		glinject_lua_pushkeyeventmodifiers(L, event);
		if (lua_pcall(L, 2, 1, 0) != 0) {
			fprintf(stderr, "error running function: %s\n",
					lua_tostring(L, -1));
		}
		if (!lua_isboolean(L, -1)) {
			fprintf(stderr,
					"should_consume_key_press_event must return a boolean\n");
		}
		Bool res = lua_toboolean(L, -1);
		lua_pop(L, 1);
		return res;
	}
	pthread_mutex_unlock(&glinject_mutex);
	return False;
}

/*
 * Handles an X11 event.
 */
void glinject_handle_x11_event(XEvent* event) {
	if (event == NULL)
		return;

	if (event->type == KeyPress) {
		pthread_mutex_lock(&glinject_mutex);
		glinject_handle_x11_key_event((XKeyEvent*) event);
		pthread_mutex_unlock(&glinject_mutex);
	}
	if (event->type == ConfigureNotify) {
		pthread_mutex_lock(&glinject_mutex);
		glinject_handle_x11_configure_notify_event(event);
		pthread_mutex_unlock(&glinject_mutex);
	}

}

/*
 * ============================================================================
 * GLX method call handlers
 * ============================================================================
 */
/*
 * Handles buffer swap. This is the method that is called at the end of each
 * frame.
 */
void glinject_handle_buffer_swap(Display* dpy, GLXDrawable drawable) {
	XEvent event;
	XCheckIfEvent(dpy, &event, glinject_check_if_event, NULL);

	pthread_mutex_lock(&glinject_mutex);
	lua_getglobal(L, "handle_buffer_swap"); /* function to be called */
	if (!lua_isfunction(L, -1)) {
		fprintf(stderr, "handle_buffer_swap is not a function!\n");
		return;
	}
	int context_id;
	GLXContext context = glinject_real_glXGetCurrentContext();
	glinject_real_glXQueryContext(dpy, context, GLX_FBCONFIG_ID, &context_id);
	lua_pushlightuserdata(L, dpy);
	lua_pushnumber(L, context_id);
	lua_pushnumber(L, drawable);
	if (lua_pcall(L, 3, 0, 0) != 0) {
		fprintf(stderr, "error running function: %s\n", lua_tostring(L, -1));
	}
	pthread_mutex_unlock(&glinject_mutex);
}

/*
 * Handles the destruction of a GLXContext. Called when glXDestroyContext is
 * called.
 */
void glinject_handle_context_destruction(Display* dpy, GLXContext context) {
	pthread_mutex_lock(&glinject_mutex);
	lua_getglobal(L, "handle_context_destruction"); /* function to be called */
	if (!lua_isfunction(L, -1)) {
		fprintf(stderr, "handle_context_destruction is not a function!\n");
		return;
	}
	int context_id;
	glinject_real_glXQueryContext(dpy, context, GLX_FBCONFIG_ID, &context_id);
	lua_pushlightuserdata(L, dpy);
	lua_pushnumber(L, context_id);
	if (lua_pcall(L, 2, 0, 0) != 0) {
		fprintf(stderr, "error running function: %s\n", lua_tostring(L, -1));
	}
	pthread_mutex_unlock(&glinject_mutex);
}

/*
 * ============================================================================
 * Method override helpers
 * ============================================================================
 */

/*
 * Returns NULL if this function isn't overriden, or the function pointer to
 * the proxy function.
 */
void* glinject_get_function_override(const char* name) {
	if (strcmp("glXSwapBuffers", name) == 0) {
		return &glXSwapBuffers;
	}
	if (strcmp("glXDestroyContext", name) == 0) {
		return &glXDestroyContext;
	}
	if (strcmp("glXGetProcAddressARB", name) == 0) {
		return &glXGetProcAddressARB;
	}
	if (strcmp("glXGetProcAddress", name) == 0) {
		return &glXGetProcAddress;
	}
	if (strcmp("XIfEvent", name) == 0) {
		return &XIfEvent;
	}
	if (strcmp("XCheckIfEvent", name) == 0) {
		return &XCheckIfEvent;
	}
	if (strcmp("XMaskEvent", name) == 0) {
		return &XMaskEvent;
	}
	if (strcmp("XNextEvent", name) == 0) {
		return &XNextEvent;
	}
	if (strcmp("XWindowEvent", name) == 0) {
		return &XWindowEvent;
	}
	if (strcmp("dlsym", name) == 0) {
		return &dlsym;
	}
	if (strcmp("dlvsym", name) == 0) {
		return &dlvsym;
	}
	return NULL ;
}

void * dlsym(void *handle, const char *name) {
	glinject_init();
	void* overload = glinject_get_function_override(name);
	if (overload != NULL)
		return overload;
	return glinject_real_dlsym(handle, name);
}

void *dlvsym(void *handle, const char *name, const char *ver) {
	glinject_init();
	void* overload = glinject_get_function_override(name);
	if (overload != NULL)
		return overload;
	return glinject_real_dlvsym(handle, name, ver);
}

/*
 * Returns the real glXGetProcAddress.
 */
gl_function_provider_type glinject_get_real_glXGetProcAddress() {
	glinject_init();
	return glinject_real_glXGetProcAddress;
}

/*
 * ============================================================================
 * Initialisation
 * ============================================================================
 */
void glinject_assert_symbol_loaded(void* symbol, const char* name) {
	if (symbol == NULL) {
		fprintf(stderr, "Couldn't find %s! dlsym returned a NULL pointer: %s\n",
				name, dlerror());
		exit(EXIT_FAILURE);
	}
}
/*
 * Links GLX and X
 */
void glinject_link_glx_and_xlib() {
	lua_getglobal(L, "CURRENT_PATH_CONFIG");
	lua_getfield(L, -1, "libGL"); // Read CURRENT_PATH_CONFIG.libGL
	if (!lua_isstring(L, -1)) {
		fprintf(stderr, "Couldn't determine the location of libGL "
				"because CURRENT_PATH_CONFIG.libGL is invalid.\n");
		exit(EXIT_FAILURE);
	}
	const char* lib_gl_path = strdup(lua_tostring(L, -1));
	lua_pop(L, 1);
	lua_getfield(L, -1, "libX11"); // Read CURRENT_PATH_CONFIG.libX11
	if (!lua_isstring(L, -1)) {
		fprintf(stderr, "Couldn't determine the location of libX11 "
				"because CURRENT_PATH_CONFIG.libX11 is invalid.\n");
		exit(EXIT_FAILURE);
	}
	const char* lib_x11_path = strdup(lua_tostring(L, -1));
	lua_pop(L, 2);

	glinject_load_glx_event_real_symbols(lib_gl_path);
	glinject_load_x_event_real_symbols(lib_x11_path);
}

/*
 * Links essential methods
 */
void glinject_init() {
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
		fprintf(stderr, "Couldn't find libdl!\n");
		exit(EXIT_FAILURE);
	}
	if (eh_find_sym(&libdl, "dlsym", (void **) &glinject_real_dlsym)) {
		fprintf(stderr, "Couldn't find dlsym in libdl!\n");
		eh_destroy_obj(&libdl);
		exit(EXIT_FAILURE);
	}
	if (eh_find_sym(&libdl, "dlvsym", (void **) &glinject_real_dlvsym)) {
		fprintf(stderr, "Couldn't find dlvsym in libdl!\n");
		eh_destroy_obj(&libdl);
		exit(EXIT_FAILURE);
	}
	eh_destroy_obj(&libdl);

	glinject_real_dlopen = (glinject_dlopen_type) glinject_real_dlsym(RTLD_NEXT,
			"dlopen");
	if (glinject_real_dlopen == NULL) {
		fprintf(stderr,
				"Couldn't find dlopen! dlsym returned a NULL pointer. %s\n",
				dlerror());
		exit(EXIT_FAILURE);
	}
}

/*
 * Loads and executes a Lua file.
 */
void glinject_run_file(const char* path) {
	int status = luaL_loadfile(L, path);

	if (status) {
		fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
		exit(EXIT_FAILURE);
	}
	int ret = lua_pcall(L, 0, 0, 0);
	if (ret != 0) {
		fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
		exit(EXIT_FAILURE);
	}
}

/*
 * Concatenates the base path with the suffix.
 */
char* glinject_join_path(const char* path, const char* suffix) {
	char* new_path = malloc(strlen(path) + strlen(suffix) + 1);
	strcpy(new_path, path);
	strcat(new_path, suffix);
	return new_path;
}

/*
 * This method is called at library initialisation.
 */
void glinject_construct() {

	pthread_mutex_lock(&glinject_mutex);
	// Link core methods.
	glinject_init();

	// Link luajit.
	const char* luajit_library_path = getenv("GLXOSD_LUAJIT_LIBRARY_PATH");
	if (luajit_library_path == NULL)
		luajit_library_path = "libluajit-5.1.so.2";
	glinject_load_luajit_symbols(luajit_library_path);

	// Create Lua state
	L = luaL_newstate();
	if (!L) {
		fprintf(stderr, "Lua initialization failed.");
		exit(EXIT_FAILURE);
	}
	// Open Lua libraries
	luaL_openlibs(L);

	// Find out the root of the GLXOSD script directory
	const char* path = getenv("GLXOSD_SCRIPTS_ROOT");
	if (path == NULL || strlen(path) == 0) {
		fprintf(stderr, "GLXOSD_SCRIPTS_ROOT is not set!");
		exit(EXIT_FAILURE);
	}
	lua_pushstring(L, path);
	lua_setfield(L, LUA_GLOBALSINDEX, "glxosdPackageRoot");

	char* bootstrap_suffix = "/glxosd/Bootstrap.lua";
	char* bootstrap_path = glinject_join_path(path, bootstrap_suffix);
	char* main_suffix = "/glxosd/Main.lua";
	char* main_path = glinject_join_path(path, main_suffix);

	// Run the bootstrap script.
	glinject_run_file(bootstrap_path);

	// Link GLX and X11 because we now know the paths to libGL
	// and libX11 thanks to the bootstrap script.
	glinject_link_glx_and_xlib();

	// Finally, run the main script. It can now use GLX and X11.
	glinject_run_file(main_path);

	free(bootstrap_path);
	free(main_path);
	pthread_mutex_unlock(&glinject_mutex);

}

/*
 * Destruction
 */
void glinject_destruct() {
	lua_close(L);
}
