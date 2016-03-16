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

local sens = require("ffi/libsensors")
local DummyStatistics = require("plugins/StatisticsOSD/dataproviders/DummyDataProvider")
local MarkupElement = require("rendering/MarkupElement")
if sens == nil then
	return DummyStatistics
end

sens.sensors_init(nil);


local LibsensorsDataProvider = {
	}
LibsensorsDataProvider.__index = LibsensorsDataProvider;

function LibsensorsDataProvider:begin_frame ()

end

function LibsensorsDataProvider:end_frame ()

end

function LibsensorsDataProvider:begin_timespan ()

end

local function subfeatures(chip, feature)
	local subfeature_number = ffi_types.int_ref();
	local subfeatures = {}
	while true do
		local subfeature = sens.sensors_get_all_subfeatures(chip, feature, subfeature_number)
		if subfeature == nil then
			break
		end

		local name = ffi.string(subfeature[0].name)
		local val = ffi_types.double_ref()
		sens.sensors_get_value(chip, subfeature[0].number, val)
		subfeatures[name] = {
			type=LIBSENSORS_SUBFEATURE_TYPES[tonumber(subfeature[0].type)],
			value = val[0]
		}
	end
	return subfeatures
end
local function features(chip)
	local feature_number = ffi_types.int_ref();
	local features = {}
	while true do
		local feature = sens.sensors_get_features(chip, feature_number)
		if feature == nil then
			break
		end
		local name = ffi.string(feature[0].name)
		local label_s = sens.sensors_get_label(chip,feature[0])
		if label_s ~= nil then
			local label = ffi.string(label_s);
			ffi.C.free(label_s)
			features[name] = {
				label = label,
				subfeatures = subfeatures(chip, feature)
			}
		end
	end
	return features
end

local function chips()
	local chip_number = ffi_types.int_ref();
	local chips = {}
	while true do
		local chip = sens.sensors_get_detected_chips(nil, chip_number)
		if chip == nil then
			break
		end
		local chipNameBuffer = ffi_types.char_arr(256);
		local num = sens.sensors_snprintf_chip_name(chipNameBuffer, 256, chip);
		local name = ffi.string(chipNameBuffer, num)
		chips[chip[0].addr] = {
			name=name,
			features = features(chip)
		}
	end
	return chips
end

function LibsensorsDataProvider:end_timespan ()
end

function LibsensorsDataProvider:get_text()
	local status, res = pcall(function()
		local tbl = self.config.formatter_function(self, chips(), MarkupElement)
		assert(tbl and type(tbl) == "table", "The Libsensors data provider formatter function must return a table of elements!")
		tbl = remove_nils_from_array(tbl);
		for _,v in ipairs(tbl) do
			check_class(v, "MarkupElement")
		end
		return tbl
	end)
	if not status then
		log_error("Libsensors data provider plugin error: formatter function threw and error: " .. res)
		return {
			MarkupElement.new({
				text ="Libsensors data provider plugin error.\nPlease check the log.\n"
			})
		}
	end
	return res
end

function LibsensorsDataProvider.new(config)
	local self ={}
	setmetatable(self, LibsensorsDataProvider)
	self.config = config
	return self
end

return LibsensorsDataProvider
