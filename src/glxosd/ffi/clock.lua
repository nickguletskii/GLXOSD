--[[
Copyright (C) 2016 Nick Guletskii

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
]]

local ffi = require("ffi")

CLOCK_MONOTONIC = 1

ffi.cdef[[

typedef struct timespec
{
	int tv_sec;
	long tv_nsec;
} timespec;

int clock_gettime(int clk_id, struct timespec *tp);

]]

get_monotonic_time_nanoseconds = function()
	local ts = ffi_types.timespec_ref();
	ffi.C.clock_gettime(CLOCK_MONOTONIC, ts);
	local num = ts[0].tv_sec * 1000000000 + ts[0].tv_nsec;
	return num
end

ms_to_ns = function(time)
	return time * 1000000
end

ns_to_ms = function(time)
	return tonumber(time) / 1000000
end