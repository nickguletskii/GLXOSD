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
#include "luajit_dynamic.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>

void glinject_load_luajit_symbols(const char* path) {
	void *handle = dlopen(path, RTLD_LOCAL | RTLD_LAZY);
	if (!handle) {
		fprintf(stderr, "Couldn't load luajit symbols: %s\n", dlerror());
		exit(EXIT_FAILURE);
	}

	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, lua_close);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, lua_createtable);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, lua_getfield);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, lua_type);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, lua_pcall);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, lua_settop);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, lua_pushboolean);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, lua_pushlightuserdata);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, lua_pushnumber);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, lua_pushstring);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, lua_setfield);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, lua_toboolean);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, lua_tolstring);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, luaL_openlibs);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, lua_isstring);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, luaL_loadfile);
	GLINJECT_LOAD_SYMBOL_USING_DLSYM(handle, luaL_newstate);

}
