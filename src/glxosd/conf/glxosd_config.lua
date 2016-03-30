--[[
	The main configuration file for GLXOSD. This is a basic Lua script that sets
	the GLXOSD_CONFIG global to the configuration to be used.

	To customise paths, please see paths.lua.
]]
--------------------------------------------------------------------------------
-- SECTION configuration file includes                                        --
--------------------------------------------------------------------------------
ConfigurationManager.config_file("paths.lua")

--------------------------------------------------------------------------------
-- SECTION Shared variables                                                   --
--------------------------------------------------------------------------------
--[[
	The master opacity. This is the coefficient for all opacity values.
]]
local MASTER_OPACITY = 1

--[[
	A table of colors that will be used later on in the config.
	Each color is represented by a table that has four keys: r, g, b and a.
]]
local COLORS_CONFIG = {
	-- The default text color
	DEFAULT = {r = 1, g = 0, b = 1, a = 1 * MASTER_OPACITY},
	-- The color of the text outline
	OUTLINE = {r = 0, g = 0, b = 0, a = 1 * MASTER_OPACITY},
	-- The color to be used to highlight critical temperatures, low FPS, etc...
	BAD_VALUE = {r = 1, g = 0, b = 0, a = 1 * MASTER_OPACITY},
	-- The color to be used to highlight high temperatures and other warnings
	WARNING_VALUE = {r = 1, g = 1, b = 0, a = 1 * MASTER_OPACITY},
	-- The color to be used to highlight acceptable temperatures and normal FPS
	GOOD_VALUE = {r = 0, g = 1, b = 0, a = 1 * MASTER_OPACITY},
	-- Completely transparent
	NONE = {r = 0, g = 0, b = 0, a = 0 * MASTER_OPACITY}
}

--[[
	The default config for all OSD section headers.
]]
local DEFAULT_HEADER_CONFIG = {
	-- If set to true, display section headers.
	enabled = true,

	--[[
		General markup configuration.
		For available options, please see the section about markup in the
		documentation.
	]]
	color = COLORS_CONFIG.DEFAULT,
	font_size = 20,
	underline = {
		enabled = true,
		color = COLORS_CONFIG.DEFAULT
	}
}

--------------------------------------------------------------------------------
-- SECTION OSD configuration                                                  --
--------------------------------------------------------------------------------
--[[
	The config for the OSD.
]]
local OSD_CONFIG = {
	--[[
		The time in milliseconds between OSD redraws. This is done to lower the
        overhead because reuploading the text to the GPU every frame is
        expensive.
	]]
	refresh_time = 1000,

	--[[
		The alignment of the OSD's bounding box in respect to the view.

		Acceptable values for align_to_h:
		* left - aligns the left side of the OSD to the left side of the view
		* right - aligns the right side of the OSD to the right side of the view
		* right_absolute - aligns the left side of the OSD to the right side of
		                   the view

		Acceptable values for align_to_v:
		* top - aligns the top side of the OSD to the top side of the view
		* bottom - aligns the bottom side of the OSD to the bottom side of the
		           view
		* bottom_absolute - aligns the top side of the OSD to the bottom side of
		                    the view
	]]
	align_to_h = "left",
	align_to_v = "top",

	--[[
		The position of the OSD in respect to the corner it's aligned to.
	]]
	offset_x = 0,
	offset_y = 0,

	--[[
        The font to be used. This is a Fontconfig font query string.
	]]
	font = "Ubuntu:Bold",

	--[[
		The text size.
	]]
	font_size = 16,

	--[[
		Text alignment in respect to the OSD's bounding box.
		Possible values: left, right, center
	]]
	text_alignment = "left",

	--[[
		Outline configuration
	]]
	outline = {
		--[[
			The type of the outline. Possible values include:
			* none - no outline
			* outer - outer rim of the symbols
			* inner - inner rim of the symbols
			* inside - the interior of the symbols
		]]
		type = "outer",
		--[[
			The thickness of the outline.
		]]
		thickness = 0.5,
		--[[
			The color of the outline.
		]]
		color = COLORS_CONFIG.OUTLINE
	},

	--[[
		Value used for gamma correction.
	]]
	gamma = 1,

	--[[
		LCD filtering may improve the appearance of the text.
		Set this setting to true to enable LCD filtering and to false to disable
		it.
	]]
	lcd_filter_enabled = true,

	--[[
		Settings for the underline, overline and strikethrough.
		Pretty self-explanatory.
	]]
	underline = {
		enabled = false,
		color = COLORS_CONFIG.NONE
	},
	overline = {
		enabled = false,
		color = COLORS_CONFIG.NONE
	},
	strikethrough = {
		enabled = false,
		color = COLORS_CONFIG.NONE
	},

	--[[
		The color of the text.
	]]
	color = COLORS_CONFIG.DEFAULT,

	--[[
		The background color of each symbol.
	]]
	background_color = COLORS_CONFIG.NONE,

	--[[
		The key combination to toggle the OSD visibility. Set to nil to disable.
	]]
	toggle_key_combo = {
		--[[
			The name of the main key.
		]]
		main_key = "F10",

		--[[
			The list of modifier keys.
			Possible values include: shift, alt, control, caps.
		]]
		modifiers = {"shift"}
	},

	--[[
		The list of data providers to collect data from.
		Each data provider is described by three key-value pairs:
		* path - the path to the data provider's main file
		* enabled - whether this data provider is currently enabled or not
		* config - the configuration to pass to the data provider
	]]
	osd_data_providers = {
		{
			path = "plugins/OSD/dataproviders/FrameDataProvider",
			enabled = true,
			config = {
				--[[
					If the FPS is lower than this setting, the default
					formatter_function will highlight the FPS as a bad value.
				]]
				low_fps = 55,

				--[[
					The FrameDataProvider provides the data from the last
					complete chunk (timespan) of n milliseconds, where n is the
                    value of refresh_time.
				]]
				refresh_time = 3000,

				--[[
					The color of acceptable values (e.g. high enough FPS).
				]]
				acceptable_value_color = COLORS_CONFIG.GOOD_VALUE,

				--[[
					The color of unacceptable values (e.g. low FPS).
				]]
				unacceptable_value_color = COLORS_CONFIG.BAD_VALUE,

				--[[
					The ordered set of the statistics to be displayed.
					You can reorder or remove these to customise in what order
                    and what statistics are displayed.
				]]
				data_order = {
					-- Frames Per Second (over the last timespan).
					"FPS",
					-- Average Frame Duration (over the timespan).
					"AFD",
					--[[
						OSD Average Time Spent Per Frame - time spent by GLXOSD
						per frame on average over the last timespan.
					]]
					"OSDATSPF"
				},

				--[[
					The function used for formatting the text on the OSD.
					This can be used to completely customize what data is shown
					and how it is formatted.
				]]
				formatter_function = ConfigurationManager
					.config_file("OSD/dataproviders/FrameDataProvider/formatterfunction.lua"),

				--[[
					The style of the header of the section.
				]]
				header_style = DEFAULT_HEADER_CONFIG
			}
		},
		{
			path = "plugins/OSD/dataproviders/LibsensorsDataProvider",
			enabled = true,
			config = {
				--[[
					The function used to filter out chips that you don't want
					to be displayed in the OSD.

					This function has two parameters:
					* self - the instance of the data provider
					* chip - the chip to be considered.

                    For more information, please see the default formatter
                    function located in
                    OSD/dataproviders/LibsensorsDataProvider/formatterfunction.lua
				]]
				chip_filter_function = function(self, chip)
					return true
				end,

				--[[
					The function used to filter out features that you don't want
					to be displayed in the OSD.

					This function has three parameters:
					* self - the instance of the data provider
					* chip - the chip to be considered.

                    For more information, please see the default formatter
                    function located in
                    OSD/dataproviders/LibsensorsDataProvider/formatterfunction.lua
				]]
				feature_filter_function = function(self, chip, feature)
					--[[
						For example, you can use the following line
						to hide per-core temperatures on Intel CPUs:
					]]
					--return not string.match(feature.label, "Core.*")
					return true
				end,

				--[[
                    If the temperature is above the maximum temperature for the
                    device minus max_temperature_warning_threshold, the
                    temperature will be highlighted.
				]]
				max_temperature_warning_threshold = 10,

				--[[
                    The color to be used in case the maximum acceptable
                    temperature is unknown.
                ]]
				default_temperature_color = COLORS_CONFIG.DEFAULT,

				--[[
                    The color to be used if the color is under the maximum
                    temperature for the device minus the warning threshold.
                ]]
				good_temperature_color = COLORS_CONFIG.GOOD_VALUE,

				--[[
                    The color to be used if the color is over the maximum
                    temperature for the device minus the warning threshold, but
                    under the maximum temperature for the device.
                ]]
				warning_temperature_color = COLORS_CONFIG.WARNING_VALUE,

				--[[
                    The color to be used if the color is over the maximum
                    temperature for the device.
                ]]
				overheating_temperature_color = COLORS_CONFIG.BAD_VALUE,

				--[[
					The function used for formatting the text on the OSD.
					This can be used to completely customize what data is shown
					and how it is formatted.
				]]
				formatter_function = ConfigurationManager
					.config_file("OSD/dataproviders/LibsensorsDataProvider/formatterfunction.lua"),

				--[[
    				The style of the header of the section.
    			]]
				header_style = DEFAULT_HEADER_CONFIG
			}
		},
		{
			path = "plugins/OSD/dataproviders/NVMLDataProvider",
			enabled = true,
			config = {
				--[[
                    The color to be used in case the slowdown and shutdown
                    temperatures are unknown.
                ]]
				default_temperature_color = COLORS_CONFIG.DEFAULT,

				--[[
                    The color to be used to represent normal temperatures.
                ]]
				normal_temperature_color = COLORS_CONFIG.GOOD_VALUE,

				--[[
                    The color to be used to represent temperatures that cause
                    a slowdown of the graphics card.
                ]]
				slowdown_temperature_color = COLORS_CONFIG.WARNING_VALUE,

				--[[
                    The color to be used to represent temperatures that may
                    cause a graphics card shutdown.
                ]]
				shutdown_temperature_color = COLORS_CONFIG.BAD_VALUE,

				--[[
					The ordered set of the statistics to be displayed.
					You can reorder or remove these to customise in what order
                    and what statistics are displayed.
				]]
				data_order = {
					"temperature",
					"gpu_utilisation",
					"memory_utilisation",
					"graphics_clock",
					"memory_clock",
					"sm_clock",
				},

				--[[
					The function used for formatting the text on the OSD.
					This can be used to completely customize what data is shown
					and how it is formatted.
				]]
				formatter_function = ConfigurationManager
					.config_file("OSD/dataproviders/NVMLDataProvider/formatterfunction.lua"),

				--[[
    				The style of the header of the section.
    			]]
				header_style = DEFAULT_HEADER_CONFIG
			}
		}
	}
}

--------------------------------------------------------------------------------
-- SECTION Time recorder configuration                                        --
--------------------------------------------------------------------------------
--[[
	The config for the frame timing logging (benchmarking) plugin.
]]
TIME_RECORDER_CONFIG = {
	--[[
        The directory to write the time logs to.
    ]]
	benchmark_output_directory = "/tmp/",

	--[[
        The key combination to toggle the OSD visibility. Set to nil to disable.
    ]]
	start_benchmark_key = {
		--[[
            The name of the main key.
        ]]
		main_key = "F9",

		--[[
            The list of modifier keys.
            Possible values include: shift, alt, control, caps.
        ]]
		modifiers = {"shift"}
	},

	--[[
        The key combination to toggle the OSD visibility. Set to nil to disable.
        You can use the same key as start_benchmark_key.
    ]]
	end_benchmark_key = {
		--[[
            The name of the main key.
        ]]
		main_key = "F9",

		--[[
            The list of modifier keys.
            Possible values include: shift, alt, control, caps.
        ]]
		modifiers = {"shift"}
	},

	--[[
        If this is set to true, GLXOSD will not execute potentially expensive
        tasks such as rendering the OSD while logging frame timings.
    ]]
	enable_minimise_overhead_mode_during_benchmark = true
}

--------------------------------------------------------------------------------
-- SECTION global configuration                                               --
--------------------------------------------------------------------------------
GLXOSD_CONFIG = {

		--[[
		The list of GLXOSD plugins.
		Each plugin is described by three key-value pairs:
		* path - the path to the plugin's main file
		* enabled - whether this plugin is currently enabled or not
		* config - the configuration to pass to the plugin
	]]
		plugins = {
			{
				path = "plugins/OSD/OSD",
				enabled = true,
				config = OSD_CONFIG
			},
			{
				path = "plugins/TimeRecorder/TimeRecorder",
				enabled = true,
				config = TIME_RECORDER_CONFIG
			}
		}
}

return GLXOSD_CONFIG
