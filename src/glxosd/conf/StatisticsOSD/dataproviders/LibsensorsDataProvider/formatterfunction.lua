return function(self, chips, el)
	local res = {}
	local chip_views = {}
	for kchip, chip in pairs(chips) do
		if self.config.chip_filter_function(self, chip) then
			local chip_temperature_found = false
			local feature_views = {}
			for kfeature, feature in pairs(chip.features) do
				if self.config.feature_filter_function(self, chip, feature) then
					local feature_temperature_found = false
					local current_temperature = nil
					local current_max_temperature = nil
					for _, subfeature in pairs(feature.subfeatures) do
						if subfeature.type == "TEMP_INPUT" then
							feature_temperature_found = true
							chip_temperature_found = true
							current_temperature = subfeature.value
						elseif  subfeature.type == "TEMP_MAX" then
							current_max_temperature = subfeature.value
						end
					end
					if feature_temperature_found then
						table.insert(feature_views, {
							label = feature.label,
							id = kfeature,
							current_temperature = current_temperature,
							current_max_temperature = current_max_temperature
						})
					end
				end
			end

			table.sort(feature_views, function(l, r)
				if l.label == r.label then
					return l.id < r.id
				end
				return l.label < r.label
			end)

			if chip_temperature_found then
				table.insert(chip_views, {
					name = chip.name,
					id= kchip,
					features = feature_views
				})
			end
		end
	end
	table.sort(chip_views, function(l, r)
		if l.name == r.name then
			return l.id < r.id
		end
		return l.name < r.name
	end)

	for _, chip in pairs(chip_views) do
		table.insert_all(res,
			el.new({text = chip.name}),
			el.newline
		)
		for _, feature in pairs(chip.features) do
			local heat_status = "unknown"
			if feature.current_max_temperature then
				heat_status = "good"
				if feature.current_temperature >= feature.current_max_temperature -self.config.max_temperature_warning_threshold then
					heat_status = "warning"
				end
				if feature.current_temperature >= feature.current_max_temperature then
					heat_status = "overheating"
				end
			end

			local color_map = {
				unknown = self.config.default_temperature_color,
				good = self.config.good_temperature_color,
				warning = self.config.warning_temperature_color,
				overhearing = self.config.overheating_temperature_color
			}
			local temperature_color = color_map[heat_status]
			table.insert_all(res,
				el.new({text = " " .. feature.label..": "}),
				el.new({
					text=tostring(feature.current_temperature),
					color=temperature_color
				}),
				el.newline
			)
		end
	end
	return res
end
