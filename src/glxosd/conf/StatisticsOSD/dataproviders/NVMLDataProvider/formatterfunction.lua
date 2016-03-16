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
]]

local formattingutil = require("util/formattingutil")

local DATA_WRITERS = {
	temperature = function(self, offset, dev, res, el)
		local heat_status = "unknown"
		if dev.temperature_thresholds then
			if dev.temperature_thresholds.slowdown or dev.temperature_thresholds.shutdown then
				heat_status="normal"
			end
			if dev.temperature_thresholds.slowdown and dev.temperature >= dev.temperature_thresholds.slowdown then
				heat_status = "slowdown"
			end
			if dev.temperature_thresholds.shutdown and dev.temperature >= dev.temperature_thresholds.shutdown then
				heat_status = "shutdown"
			end
		end

		local color_map = {
			unknown = self.config.default_temperature_color,
			normal = self.config.normal_temperature_color,
			slowdown = self.config.slowdown_temperature_color,
			shutdown = self.config.shutdown_temperature_color
		}
		local temperature_color = color_map[heat_status]
		table.insert_all(res,
			el.indent(offset+1),
			el.new({text = "Temperature: "}),
			el.new({
				text=tostring(dev.temperature).."°",
				color=temperature_color
			})
		)
		if dev.temperature_thresholds.slowdown and dev.temperature >= dev.temperature_thresholds.slowdown then
			table.insert_all(res,
				el.new({
					text = "(thrtl)",
					color=self.config.slowdown_temperature_color
				})
			)
		end
		table.insert_all(res,
			el.newline
		)
	end,
	graphics_clock = function(self, offset, dev, res, el)
		if dev.clocks and dev.clocks.graphics then
			table.insert_all(res,
				el.indent(offset+1),
				el.new({
					text = string.format("Graphics clock: %d",dev.clocks.graphics)
				}),
				el.newline
			)
		end
	end,
	sm_clock = function(self, offset, dev, res, el)
		if dev.clocks and dev.clocks.sm then
			table.insert_all(res,
				el.indent(offset+1),
				el.new({
					text = string.format("SM clock: %d", dev.clocks.sm)
				}),
				el.newline
			)
		end
	end,
	memory_clock = function(self, offset, dev, res, el)
		if dev.clocks and dev.clocks.memory then
			table.insert_all(res,
				el.indent(offset+1),
				el.new({
					text = string.format("Memory clock: %d", dev.clocks.memory)
				}),
				el.newline
			)
		end
	end,
	gpu_utilisation = function(self, offset, dev, res, el)
		if dev.utilisation then
			table.insert_all(res,
				el.indent(offset+1),
				el.new({
					text = string.format("GPU utilisation: %d%%", dev.utilisation.gpu)
				}),
				el.newline
			)
		end
	end,
	memory_utilisation = function(self, offset, dev, res, el)
		if dev.utilisation then
			table.insert_all(res,
				el.indent(offset+1),
				el.new({
					text = string.format("Memory utilisation: %d%%", dev.utilisation.memory)
				}),
				el.newline
			)
		end
	end
}

return function(self, devices, el)
	local offset = self.config.header.enabled and 1 or 0
	local header  =
		formattingutil.create_osd_section_header("GPUs", self, el)
	local res = {
		header,
		header and el.newline,
	}
	for _, dev in pairs(devices) do
		table.insert_all(res,
			el.indent(offset),
			el.new({text = dev.name}),
			el.newline
		)
		for _, data_name in ipairs(self.config.data_order) do
			DATA_WRITERS[data_name](self, offset, dev, res, el)
		end
	end
	return res
end
