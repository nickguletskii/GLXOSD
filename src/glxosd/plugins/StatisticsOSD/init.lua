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

local TextRenderer = require("rendering/TextRenderer")
local FontUtil = require("util/fontutil")
local StatisticsOSD = {
	}
StatisticsOSD.__index = StatisticsOSD;

function StatisticsOSD:each_data_provider(func)
	for i, x in ipairs(self.data_providers) do
		if x~=nil then
			local status, err = pcall(function() func(x) end)
			if not status then
				log_error("Error while acting upon data provider: ".. err)
			end
		end
	end
end

function StatisticsOSD:begin_frame()
	local current_time =  get_monotonic_time_nanoseconds();
	if(self.last_text_reset <=  current_time - self.config.refresh_time* 1000000) then
		self.rebuild_text = true
		self.has_timespan = true
		self.last_text_reset = current_time
	end
	self:each_data_provider(function(data_provider)
		data_provider:begin_frame()
	end)
end

function StatisticsOSD:end_frame()
	self:each_data_provider(function(data_provider)
		data_provider:end_frame()
	end)
end
function StatisticsOSD:render(width, height)
	if self.rendering_enabled then
		if self.rebuild_text then
			local text = {}
			self:each_data_provider(function(data_provider)
				local tbl = data_provider:get_text()
				for _,v in ipairs(tbl) do
					table.insert(text, v)
				end
			end)
			self.text_renderer:set_text(text)
			self.rebuild_text = false
		end
		self.text_renderer:render(width, height)
	end
end

local function check_key_combo(key_combo, key, modifiers)
	local modifier_set={}
	for _, modifier in pairs(key_combo.modifiers) do
		modifier_set[modifier] = true
		if not modifiers[modifier] then
			return false
		end
	end
	for modifier, val in pairs(modifiers) do
		if val and not modifier_set[modifier] then
			return false
		end
	end
	return key_combo.main_key == key
end

function StatisticsOSD:has_keyboard_combo(key, modifiers)
	return check_key_combo(self.config.toggle_key_combo, key, modifiers)
end
function StatisticsOSD:handle_key_combo(key, modifiers)
	if check_key_combo(self.config.toggle_key_combo, key, modifiers) then
		self.rendering_enabled = not self.rendering_enabled
	end
end
function StatisticsOSD:destroy()
	self.text_render:destroy()
end
function StatisticsOSD.new(config)
	local self ={}
	setmetatable(self, StatisticsOSD)

	self.config = config

	self.text_renderer = TextRenderer.new(
		config
	);
	self.data_providers = {}

	for i, data_provider in pairs(config.osd_data_providers) do
		if data_provider ~= nil and data_provider.enabled then
			table.insert(
				self.data_providers,
				require(data_provider.path)
					.new(data_provider.config))
		end
	end

	self.rendering_enabled = true
	self.has_timespan = false;
	self.last_text_reset = 0
	self.rebuild_text = true
	self.text_renderer:set_text({})
	return self
end

return StatisticsOSD;
