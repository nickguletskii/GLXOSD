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
local io = require("io")
local TextRenderer = require("rendering/TextRenderer")
local FontUtil = require("util/fontutil")
local TimeRecorder = {
	}
TimeRecorder.__index = TimeRecorder;

local benchmark_id = 0

function TimeRecorder:should_render()
	return false
end

function TimeRecorder:begin_frame()
	local current_time =  get_monotonic_time_nanoseconds();
	if self.started then
		if self.last_frame_time then
			self.file:write((tonumber((current_time - self.last_frame_time))/1000000).."\n")
			self.file:flush()
		end
		self.last_frame_time = current_time
	end
end

function TimeRecorder:end_frame()
end

function TimeRecorder:render(width, height)
end

function TimeRecorder:consumes_keyboard_combo(key, modifiers)
	return check_key_combo(self.config.start_benchmark_key, key, modifiers)
		or check_key_combo(self.config.end_benchmark_key, key, modifiers)
end


function TimeRecorder:start_benchmark()
	self.file_name = self.config.benchmark_output_directory.."/glxosd_benchmark_"..os.date().."_"..benchmark_id..".csv"
	benchmark_id = benchmark_id + 1
	self.file = io.open(self.file_name, "w")
	self.file:write("# GLXOSD frame log version (3.0)\n")
	self.file:flush()
	if self.file then
		self.started = true
		if self.config.enable_minimise_overhead_mode_during_benchmark then
			self.shared_state.minimise_overhead_mode["TimeRecoreder"] = true
		end
	end
end

function TimeRecorder:end_benchmark()
	self.file:close()
	self.started = false
	self.last_frame_time = nil
	if self.config.enable_minimise_overhead_mode_during_benchmark then
		self.shared_state.minimise_overhead_mode["TimeRecoreder"] = false
	end
end

function TimeRecorder:handle_key_combo(key, modifiers)
	if not self.started and check_key_combo(self.config.start_benchmark_key, key, modifiers) then
		self:start_benchmark()
	elseif self.started and check_key_combo(self.config.end_benchmark_key, key, modifiers) then
		self:end_benchmark()
	end
end

function TimeRecorder:destroy()
end

function TimeRecorder.new(config, shared_state)
	local self ={}
	setmetatable(self, TimeRecorder)
	self.shared_state = shared_state
	self.started = false
	self.config = config
	return self
end

return TimeRecorder;
