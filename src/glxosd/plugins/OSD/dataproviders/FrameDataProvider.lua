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

local MarkupElement = require("rendering/MarkupElement")
local FrameDataProvider = {
	}
FrameDataProvider.__index = FrameDataProvider;

function FrameDataProvider:begin_frame ()
	if not self.current_timespan_stats then
		self:begin_timespan()
	elseif self.current_timespan_stats.timespan_start
		<= get_monotonic_time_nanoseconds()-ms_to_ns(self.config.refresh_time) then
		self:end_timespan()
		self:begin_timespan()
	end

	local current_time =  get_monotonic_time_nanoseconds();
	self.previous_state = self.current_state
	self.current_frame_stats = {
		frame_start = current_time,
		frame_end = nil
	}
end

function FrameDataProvider:end_frame ()
	local current_time =  get_monotonic_time_nanoseconds()
	self.current_frame_stats.frame_end = current_time
	self.current_frame_stats.glxosd_overhead = self.current_frame_stats.frame_end - self.current_frame_stats.frame_start
	self.current_timespan_stats.frames_in_timespan_count = self.current_timespan_stats.frames_in_timespan_count + 1
	self.current_timespan_stats.glxosd_overhead_in_timespan = self.current_timespan_stats.glxosd_overhead_in_timespan + self.current_frame_stats.glxosd_overhead
end

function FrameDataProvider:begin_timespan ()
	self.current_timespan_stats = {
		timespan_start = get_monotonic_time_nanoseconds(),
		frames_in_timespan_count = 0,
		glxosd_overhead_in_timespan= 0
	}
end

function FrameDataProvider:end_timespan ()
	self.previous_timespan_stats = {
		timespan_length = get_monotonic_time_nanoseconds() - self.current_timespan_stats.timespan_start,
		frames_in_timespan_count = self.current_timespan_stats.frames_in_timespan_count,
		glxosd_overhead_in_timespan = self.current_timespan_stats.glxosd_overhead_in_timespan
	}
	self.has_data = true
end

function FrameDataProvider:get_previous_timespan_length_ms()
	if not self.previous_timespan_stats then
		return nil
	end
	return ns_to_ms(self.previous_timespan_stats.timespan_length)
end

function FrameDataProvider:get_previous_frames_in_timespan_count()
	if not self.previous_timespan_stats then
		return nil
	end
	return self.previous_timespan_stats.frames_in_timespan_count
end

function FrameDataProvider:get_previous_glxosd_overhead_in_timespan_ms()
	if not self.previous_timespan_stats then
		return nil
	end
	return ns_to_ms(self.previous_timespan_stats.glxosd_overhead_in_timespan)
end

function FrameDataProvider:get_previous_glxosd_overhead_per_frame_ms()
	if not self.previous_timespan_stats then
		return nil
	end
	return
		ns_to_ms(self.previous_timespan_stats.glxosd_overhead_in_timespan)
		/ self.previous_timespan_stats.frames_in_timespan_count
end

function FrameDataProvider:get_average_frame_duration_ms()
	return ns_to_ms(self.previous_timespan_stats.timespan_length)
			/self.previous_timespan_stats.frames_in_timespan_count
end

function FrameDataProvider:get_previous_fps()
	if not self.previous_timespan_stats then
		return nil
	end
	local frames =  tonumber(self.previous_timespan_stats.frames_in_timespan_count)
	local period = tonumber(self.previous_timespan_stats.timespan_length)
	return 1000*1000000* frames/period
end

function FrameDataProvider:get_text()
	local status, res = pcall(function()
		local tbl = self.config.formatter_function(self, MarkupElement)
		assert(tbl and type(tbl) == "table", "The frame data provider formatter function must return a table of elements!")
		tbl = remove_nils_from_array(tbl);
		for _,v in ipairs(tbl) do
			check_class(v, "MarkupElement")
		end
		return tbl
	end)
	if not status then
		log_error("Frame data provider plugin error: formatter function threw and error: " .. res)
		return {
			MarkupElement.new({
				text ="Frame data provider plugin error.\nPlease check the log.\n"
			})
		}
	end
	return res
end

function FrameDataProvider.new(config)
	local self ={}

	self.config = config

	setmetatable(self, FrameDataProvider)
	self.has_data = false
	self.current_frame_stats = nil
	self.current_timespan_stats = nil
	self.previous_timespan_stats = nil
	return self
end

return FrameDataProvider
