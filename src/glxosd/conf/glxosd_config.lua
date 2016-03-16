local MASTER_OPACITY = 1

local COLORS_CONFIG = {
	DEFAULT = {r=1,g=0,b=1,a=1 * MASTER_OPACITY},
	OUTLINE = {r=0,g=0,b=0,a=1 * MASTER_OPACITY},
	BAD_VALUE = {r=1,g=0,b=0,a=1 * MASTER_OPACITY},
	WARNING_VALUE = {r=1,g=1,b=0,a=1 * MASTER_OPACITY},
	GOOD_VALUE = {r=0,g=1,b=0,a=1 * MASTER_OPACITY},
	NONE = {r=0,g=0,b=0,a=0 * MASTER_OPACITY}
}

local DEFAULT_HEADER_CONFIG = {
	enabled=true,
	color=COLORS_CONFIG.DEFAULT,
	font_size=20,
	underline={
		enabled = true,
		color=COLORS_CONFIG.DEFAULT
	}
}

local STATISTICS_OSD_CONFIG = {
	refresh_time = 1000,
	align_to_h = "left",
	align_to_v = "top",
	font = "Ubuntu:Bold",
	font_size = 16,
	outline={
		type="outer",
		thickness=0.5,
		color=COLORS_CONFIG.OUTLINE
	},
	gamma=2,
	lcd_filter_enabled=true,
	underline={
		enabled=false,
		color=COLORS_CONFIG.NONE
	},
	overline={
		enabled=false,
		color=COLORS_CONFIG.NONE
	},
	strikethrough={
		enabled=false,
		color=COLORS_CONFIG.NONE
	},
	color=COLORS_CONFIG.DEFAULT,
	background_color=COLORS_CONFIG.NONE,
	toggle_key_combo={
		main_key="F10",
		modifiers={"shift"}
	},
	osd_data_providers = {
		{
			path = "plugins/StatisticsOSD/dataproviders/FrameDataProvider",
			enabled = true,
			config = {
				low_fps = 55,
				refresh_time = 3000,
				max_overhead_percentage = 1.5,
				acceptable_value_color=COLORS_CONFIG.GOOD_VALUE,
				unacceptable_value_color=COLORS_CONFIG.BAD_VALUE,
				data_order = {
					"FPS",
					"AFD",
					"OSDATSPF"
				},
				formatter_function = ConfigurationManager
					.config_file("StatisticsOSD/dataproviders/FrameDataProvider/formatterfunction.lua"),
				header=DEFAULT_HEADER_CONFIG
			}
		},
		{
			path = "plugins/StatisticsOSD/dataproviders/LibsensorsDataProvider",
			enabled = true,
			config = {
				chip_filter_function = function(self, chip)
					return true
				end,
				feature_filter_function = function(self, chip, feature)
					--return string.match(feature.label,"Core.*")
					return true
				end,
				max_temperature_warning_threshold=10,
				default_temperature_color=COLORS_CONFIG.DEFAULT,
				good_temperature_color=COLORS_CONFIG.GOOD_VALUE,
				warning_temperature_color=COLORS_CONFIG.WARNING_VALUE,
				overheating_temperature_color=COLORS_CONFIG.BAD_VALUE,
				formatter_function = ConfigurationManager
					.config_file("StatisticsOSD/dataproviders/LibsensorsDataProvider/formatterfunction.lua"),
				header=DEFAULT_HEADER_CONFIG
			}
		}
	}
}

GLXOSD_CONFIG = {
	plugins = {
		{
			path = "plugins/StatisticsOSD/StatisticsOSD",
			enabled = true,
			config = STATISTICS_OSD_CONFIG
		},
		{
			path = "plugins/TimeRecorder/TimeRecorder",
			enabled = true,
			config = {
				benchmark_output_directory="/home/nick/benchmarks",
				start_benchmark_key={
					main_key="F5",
					modifiers={"shift"}
				},
				end_benchmark_key={
					main_key="F6",
					modifiers={"shift"}
				},
				enable_minimise_overhead_mode_during_benchmark=true
			}
		}
	}
}

return GLXOSD_CONFIG
