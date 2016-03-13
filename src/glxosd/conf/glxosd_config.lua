local COLORS_CONFIG = {
	DEFAULT = {r=1,g=0,b=1,a=1},
	OUTLINE = {r=0,g=0,b=0,a=1},
	BAD_VALUE = {r=1,g=0,b=0,a=1},
	WARNING_VALUE = {r=1,g=1,b=0,a=1},
	GOOD_VALUE = {r=0,g=1,b=0,a=1},
	NONE = {r=0,g=0,b=0,a=0}
}
local STATISTICS_OSD_CONFIG = {
	refresh_time = 1000,
	align_to_h = "left",
	align_to_v = "top",
	font = "Ubuntu",
	font_size = 32,
	outline={
		type="inside",
		thickness=1,
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
				formatter_function = ConfigurationManager
					.config_file("StatisticsOSD/dataproviders/FrameDataProvider/formatterfunction.lua")
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
					.config_file("StatisticsOSD/dataproviders/LibsensorsDataProvider/formatterfunction.lua")
			}
		}
	}
}

GLXOSD_CONFIG = {
	plugins = {
		{
			path = "plugins/StatisticsOSD/init",
			enabled = true,
			config = STATISTICS_OSD_CONFIG
		}
	}
}

return GLXOSD_CONFIG