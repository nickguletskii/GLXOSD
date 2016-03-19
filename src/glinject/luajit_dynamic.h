/*
 * Copyright (C) 2013-2016 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef GLINJECT_LUAJIT_DYNAMIC_H_
#define GLINJECT_LUAJIT_DYNAMIC
#include "glinject.h"


/*
 * ============================================================================
 * A FRAGMENT FROM LUA.H
 * ============================================================================
 */
/******************************************************************************
* Copyright (C) 1994-2008 Lua.org, PUC-Rio.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

typedef struct lua_State lua_State;

#define LUA_GLOBALSINDEX	(-10002)

#define LUA_TNONE		(-1)

#define LUA_TNIL		0
#define LUA_TBOOLEAN		1
#define LUA_TLIGHTUSERDATA	2
#define LUA_TNUMBER		3
#define LUA_TSTRING		4
#define LUA_TTABLE		5
#define LUA_TFUNCTION		6
#define LUA_TUSERDATA		7
#define LUA_TTHREAD		8

#define lua_pop(L,n)		lua_settop(L, -(n)-1)

#define lua_newtable(L)		lua_createtable(L, 0, 0)

#define lua_register(L,n,f) (lua_pushcfunction(L, (f)), lua_setglobal(L, (n)))

#define lua_pushcfunction(L,f)	lua_pushcclosure(L, (f), 0)

#define lua_strlen(L,i)		lua_objlen(L, (i))

#define lua_isfunction(L,n)	(lua_type(L, (n)) == LUA_TFUNCTION)
#define lua_istable(L,n)	(lua_type(L, (n)) == LUA_TTABLE)
#define lua_islightuserdata(L,n)	(lua_type(L, (n)) == LUA_TLIGHTUSERDATA)
#define lua_isnil(L,n)		(lua_type(L, (n)) == LUA_TNIL)
#define lua_isboolean(L,n)	(lua_type(L, (n)) == LUA_TBOOLEAN)
#define lua_isthread(L,n)	(lua_type(L, (n)) == LUA_TTHREAD)
#define lua_isnone(L,n)		(lua_type(L, (n)) == LUA_TNONE)
#define lua_isnoneornil(L, n)	(lua_type(L, (n)) <= 0)

#define lua_pushliteral(L, s)	\
	lua_pushlstring(L, "" s, (sizeof(s)/sizeof(char))-1)

#define lua_setglobal(L,s)	lua_setfield(L, LUA_GLOBALSINDEX, (s))
#define lua_getglobal(L,s)	lua_getfield(L, LUA_GLOBALSINDEX, (s))

#define lua_tostring(L,i)	lua_tolstring(L, (i), NULL)

typedef double lua_Number;

typedef ptrdiff_t lua_Integer;

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
/*
 * ============================================================================
 * END FRAGMENT FROM LUA.H
 * ============================================================================
 */

GLINJECT_DEFINE_REAL_SYMBOL(lua_close, void, (lua_State *L));
GLINJECT_DEFINE_REAL_SYMBOL(lua_createtable, void,
		(lua_State *L, int narr, int nrec));
GLINJECT_DEFINE_REAL_SYMBOL(lua_getfield, void,
		(lua_State *L, int idx, const char *k));
GLINJECT_DEFINE_REAL_SYMBOL(lua_type, int, (lua_State *L, int idx));
GLINJECT_DEFINE_REAL_SYMBOL(lua_pcall, int,
		(lua_State *L, int nargs, int nresults, int errfunc));
GLINJECT_DEFINE_REAL_SYMBOL(lua_settop, void, (lua_State *L, int idx));
GLINJECT_DEFINE_REAL_SYMBOL(lua_pushboolean, void, (lua_State *L, int b));
GLINJECT_DEFINE_REAL_SYMBOL(lua_pushlightuserdata, void,
		(lua_State *L, void *p));
GLINJECT_DEFINE_REAL_SYMBOL(lua_pushnumber, void, (lua_State *L, lua_Number n));
GLINJECT_DEFINE_REAL_SYMBOL(lua_pushstring, void,
		(lua_State *L, const char *s));
GLINJECT_DEFINE_REAL_SYMBOL(lua_setfield, void,
		(lua_State *L, int idx, const char *k));
GLINJECT_DEFINE_REAL_SYMBOL(lua_toboolean, int, (lua_State *L, int idx));
GLINJECT_DEFINE_REAL_SYMBOL(lua_tolstring, const char *,
		(lua_State *L, int idx, size_t *len));
GLINJECT_DEFINE_REAL_SYMBOL(luaL_openlibs, void, (lua_State *L));
GLINJECT_DEFINE_REAL_SYMBOL(lua_isstring, int, (lua_State *L, int idx));

GLINJECT_DEFINE_REAL_SYMBOL(luaL_loadfile, int,
		(lua_State *L, const char *filename));
GLINJECT_DEFINE_REAL_SYMBOL(luaL_newstate, lua_State*, (void));

void glinject_load_luajit_symbols(const char* path);

#endif
