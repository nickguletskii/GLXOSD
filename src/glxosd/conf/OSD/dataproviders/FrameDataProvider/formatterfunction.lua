local formattingutil = require("util/formattingutil")

--[[
	A table that maps the data_order setting to the functions that write to the OSD.
]]
local DATA_WRITERS = {
	FPS = function(self, offset, osd_document, el)
		local fps = self:get_previous_fps()
		local fps_color = self.config.acceptable_value_color

		if fps<=self.config.low_fps then
			fps_color = self.config.unacceptable_value_color
		end

		table.insert_all(osd_document,
			el.new({text="FPS: "}),
			el.new({
				text=string.format("%.2f",fps),
				color=fps_color
			}),
			el.newline
		)
	end,
	AFD = function(self, offset, osd_document, el)
		local average_frame_duration = self:get_average_frame_duration_ms()

		table.insert_all(osd_document,
			el.indent(offset),
			el.new({
				text=string.format("AFD: %.2f ms", average_frame_duration)
			}),
			el.newline
		)
	end,
	OSDATSPF = function(self, offset, osd_document, el)
		local glxosd_impact = self:get_previous_glxosd_overhead_per_frame_ms()

		table.insert_all(osd_document,
			el.indent(offset),
			el.new({
				text=string.format("OSDATSPF: %.2f ms", glxosd_impact)
			}),
			el.newline
		)
	end
}
return function(self, el)
	-- If the header isn't enabled, don't add any indentation in each line.
	local offset = self.config.header_style.enabled and 1 or 0
	
	local header  =
		formattingutil.create_osd_section_header("Frame timings", self, el)
	
	if not self.has_data then
		return {
			header,
			header and el.newline,
			el.indent(offset),
			el.new({
				text="Collecting frame data..."
			}),
			el.newline
		}
	end

	local fps = self:get_previous_fps()

	local glxosd_impact = self:get_previous_glxosd_overhead_per_frame_ms()

	local osd_document = {
		header,
		header and el.newline,
	}
	for _, data_name in ipairs(self.config.data_order) do
		DATA_WRITERS[data_name](self, offset, osd_document, el)
	end
	return osd_document
end
