return function(self, el)
	if not self.has_data then
		return {
			el.new({
				text="Collecting frame data..."
			}),
			el.newline
		}
	end

	local fps = self:get_previous_fps()

	local fps_color = self.config.acceptable_value_color
	if fps<=self.config.low_fps then
		fps_color = self.config.unacceptable_value_color
	end

	local glxosd_impact = self:get_previous_glxosd_overhead_percent()

	local overhead_color = self.config.acceptable_value_color
	if glxosd_impact > self.config.max_overhead_percentage then
		overhead_color = self.config.unacceptable_value_color
	end

	return {
		el.new({text="FPS: ",
			font_size=16}),
		el.new({
			text=string.format("%.2f",fps),
			color=fps_color
		}),
		el.newline,
		el.new({text="GLXOSD impact: "}),
		el.new({
			text=string.format("%3.1f%%",glxosd_impact),
			color=overhead_color
		}),
		el.newline
	}
end
