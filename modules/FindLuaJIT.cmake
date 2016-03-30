#.rst:
# FindLuaJIT
# ---------
#
#
#
# Locate LuaJIT library This module defines
#
# ::
#
#   LUAJIT_FOUND, if false, do not try to link to LuaJIT
#   LUAJIT_LIBRARIES
#   LUAJIT_INCLUDE_DIR, where to find lua.h
#   LUAJIT_VERSION_STRING, the version of Lua found (since CMake 2.8.8)

#=============================================================================
# Modified FindLua51.cmake from CMake.
#=============================================================================
# Copyright 2016 Nick Guletskii
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# 
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
# 
# * Neither the name of the copyright holder nor the names of its
#   contributors may be used to endorse or promote products derived
#   from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#=============================================================================
# Copyright 2000-2015 Kitware, Inc.
# Copyright 2000-2011 Insight Software Consortium
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# 
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
# 
# * Neither the names of Kitware, Inc., the Insight Software Consortium,
#   nor the names of their contributors may be used to endorse or promote
#   products derived from this software without specific prior written
#   permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#=============================================================================

find_path(LUAJIT_INCLUDE_DIR luajit.h
  HINTS
    ENV LUAJIT_DIR
  PATH_SUFFIXES include/luajit-2.0/ include/luajit2.0/ include/luajit include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
)

find_library(LUAJIT_LIBRARY
  NAMES lua51 lua5.1 lua-5.1 lua
  HINTS
    ENV LUA_DIR
  PATH_SUFFIXES lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /sw
  /opt/local
  /opt/csw
  /opt
)

if(LUAJIT_LIBRARY)
  # include the math library for Unix
  if(UNIX AND NOT APPLE AND NOT BEOS AND NOT HAIKU)
    find_library(LUAJIT_MATH_LIBRARY m)
    set( LUAJIT_LIBRARIES "${LUAJIT_LIBRARY};${LUAJIT_MATH_LIBRARY}" CACHE STRING "Lua Libraries")
  # For Windows and Mac, don't need to explicitly include the math library
  else()
    set( LUAJIT_LIBRARIES "${LUAJIT_LIBRARY}" CACHE STRING "Lua Libraries")
  endif()
endif()

if(LUAJIT_INCLUDE_DIR AND EXISTS "${LUAJIT_INCLUDE_DIR}/luajit.h")
  file(STRINGS "${LUAJIT_INCLUDE_DIR}/luajit.h" LUAJIT_VERSION_STRING REGEX "^#define[ \t]+LUAJIT_VERSION[ \t]+\"LuaJIT .+\"")

  string(REGEX REPLACE "^#define[ \t]+LUAJIT_VERSION[ \t]+\"Lua ([^\"]+)\".*" "\\1" LUAJIT_VERSION_STRING "${LUAJIT_VERSION_STRING}")
  unset(LUAJIT_VERSION_STRING)
endif()

# handle the QUIETLY and REQUIRED arguments and set LUAJIT_FOUND to TRUE if
# all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LuaJIT
                                  REQUIRED_VARS LUAJIT_LIBRARIES LUAJIT_INCLUDE_DIR
                                  VERSION_VAR LUA_VERSION_STRING)

mark_as_advanced(LUAJIT_INCLUDE_DIR LUAJIT_LIBRARIES LUAJIT_LIBRARY LUAJIT_MATH_LIBRARY)

