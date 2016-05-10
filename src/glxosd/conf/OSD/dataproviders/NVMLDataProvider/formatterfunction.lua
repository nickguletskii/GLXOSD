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

--[[
	A table that maps the data_order setting to the functions that write to the OSD.
]]
local DATA_WRITERS = {
	temperature = function(self, offset, dev, osd_document, el)
		-- Determine if the temperature is normal or if the graphics card is overheating.
		local heat_status = "unknown"
		if dev.temperature_thresholds then
			if
				dev.temperature_thresholds.slowdown
				or dev.temperature_thresholds.shutdown
			then
				heat_status="normal"
			end

			if
				dev.temperature_thresholds.slowdown
				and dev.temperature >= dev.temperature_thresholds.slowdown
			then
				heat_status = "slowdown"
			end

			if
				dev.temperature_thresholds.shutdown
				and dev.temperature >= dev.temperature_thresholds.shutdown
			then
				heat_status = "shutdown"
			end
		end

		local color_map = {
			unknown = self.config.default_temperature_color,
			normal = self.config.normal_temperature_color,
			slowdown = self.config.slowdown_temperature_color,
			shutdown = self.config.shutdown_temperature_color
		}

		-- Write the temperature to the OSD.
		table.insert_all(osd_document,
			el.indent(offset+1),
			el.new({text = formattingutil.get_format(self, "temperature", "Temperature: ")}),
			el.new({
				text=string.format("%d°",dev.temperature),
				color=color_map[heat_status]
			})
		)

		-- Show a warning in the OSD if the graphics card is being throttled.
		if
			dev.temperature_thresholds.slowdown
			and dev.temperature >= dev.temperature_thresholds.slowdown
		then
			table.insert_all(osd_document,
				el.new({
					text = formattingutil.get_format(self, "throttled", "(thrtl)"),
					color=self.config.slowdown_temperature_color
				})
			)
		end

		table.insert_all(osd_document,
			el.newline
		)
	end,
	graphics_clock = function(self, offset, dev, osd_document, el)
		if
			dev.clocks
			and dev.clocks.graphics
		then
			table.insert_all(osd_document,
				el.indent(offset+1),
				el.new({
					text = string.format(formattingutil.get_format(self, "graphics_clock", "Graphics clock: %d"),dev.clocks.graphics)
				}),
				el.newline
			)
		end
	end,
	sm_clock = function(self, offset, dev, osd_document, el)
		if
			dev.clocks
			and dev.clocks.sm
		then
			table.insert_all(osd_document,
				el.indent(offset+1),
				el.new({
					text = string.format(formattingutil.get_format(self, "sm_clock", "SM clock: %d"), dev.clocks.sm)
				}),
				el.newline
			)
		end
	end,
	memory_clock = function(self, offset, dev, osd_document, el)
		if dev.clocks and dev.clocks.memory then
			table.insert_all(osd_document,
				el.indent(offset+1),
				el.new({
					text = string.format(formattingutil.get_format(self, "memory_clock", "Memory clock: %d"), dev.clocks.memory)
				}),
				el.newline
			)
		end
	end,
	gpu_utilisation = function(self, offset, dev, osd_document, el)
		if dev.utilisation then
			table.insert_all(osd_document,
				el.indent(offset+1),
				el.new({
					text = string.format(formattingutil.get_format(self, "gpu_utilisation", "GPU utilisation: %d%%"), dev.utilisation.gpu)
				}),
				el.newline
			)
		end
	end,
	memory_utilisation = function(self, offset, dev, osd_document, el)
		if dev.utilisation then
			table.insert_all(osd_document,
				el.indent(offset+1),
				el.new({
					text = string.format(formattingutil.get_format(self, "memory_utilisation", "Memory utilisation: %d%%"), dev.utilisation.memory)
				}),
				el.newline
			)
		end
	end
}

return function(self, devices, el)
	-- If the header isn't enabled, don't add any indentation in each line.
	local offset = self.config.header_style.enabled and 1 or 0

	local header  =
		formattingutil.create_osd_section_header(formattingutil.get_format(self, "header", "GPUs"), self, el)

	local osd_document = {
		header,
		header and el.newline,
	}

	for _, dev in pairs(devices) do
		-- Write device name
		table.insert_all(osd_document,
			el.indent(offset),
			el.new({text = dev.name}),
			el.newline
		)

		-- Write device data
		for _, data_name in ipairs(self.config.data_order) do
			DATA_WRITERS[data_name](self, offset, dev, osd_document, el)
		end
	end
	return osd_document
end
