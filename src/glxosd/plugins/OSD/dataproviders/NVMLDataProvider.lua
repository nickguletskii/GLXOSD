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

local ffi = require("ffi")
local nvml = require("ffi/nvml")
local DummyStatistics = require("plugins/OSD/dataproviders/DummyDataProvider")
local MarkupElement = require("rendering/MarkupElement")
if not nvml then
	return DummyStatistics
end
nvml.nvmlInit()


local NVMLDataProvider = {
	}
NVMLDataProvider.__index = NVMLDataProvider;

function NVMLDataProvider:begin_frame ()

end

function NVMLDataProvider:end_frame ()

end

function NVMLDataProvider:begin_timespan ()

end

function NVMLDataProvider:end_timespan ()
end

local function get_devices()
	local count_ref = ffi_types.unsigned_int_ref();
	nvml.nvmlDeviceGetCount(count_ref)
	local count = tonumber(count_ref[0])
	local devices = {}
	for i = 0, count-1 do
		local dev_ref = ffi_types.nvmlDevice_t_ref()
		nvml.nvmlDeviceGetHandleByIndex(i, dev_ref)
		local dev = dev_ref[0]

		local d = {}

		local uint_buf = ffi_types.unsigned_int_ref();
		local char_buf = ffi_types.char_arr(80);
		local memory_buf = ffi_types.nvmlMemory_t_ref();
		local utilisation_buf = ffi_types.nvmlUtilization_t_ref();

		local main_chars_found = true
		local uuid;

		if not nvml_expect(nvml.nvmlDeviceGetUUID(dev, char_buf, 80)) then
			main_chars_found = false
		else
			uuid = ffi.string(char_buf)
			d.uuid = uuid
		end
		if not nvml_expect(nvml.nvmlDeviceGetName(dev, char_buf, 80)) then
			main_chars_found = false
		else
			d.name = ffi.string(char_buf)
		end

		-- NAME
		if main_chars_found then
			d.name = ffi.string(char_buf)

			-- TEMPERATURE
			if nvml_expect(
				nvml.nvmlDeviceGetTemperature(dev, nvml.NVML_TEMPERATURE_GPU, uint_buf)
			) then
				d.temperature = uint_buf[0]
			end

			-- TEMPERATURE THRESHOLDS
			d.temperature_thresholds = {}
			if nvml_expect(
				nvml.nvmlDeviceGetTemperatureThreshold(dev, nvml.NVML_TEMPERATURE_THRESHOLD_SLOWDOWN, uint_buf)
			) then
				d.temperature_thresholds.slowdown = uint_buf[0]
			end
			if nvml_expect(
				nvml.nvmlDeviceGetTemperatureThreshold(dev, nvml.NVML_TEMPERATURE_THRESHOLD_SHUTDOWN, uint_buf)
			) then
				d.temperature_thresholds.shutdown = uint_buf[0]
			end

			-- CLOCKS
			d.clocks = {}
			if nvml_expect(
				nvml.nvmlDeviceGetClockInfo(dev, nvml.NVML_CLOCK_GRAPHICS, uint_buf)
			) then
				d.clocks.graphics = uint_buf[0]
			end
			if nvml_expect(
				nvml.nvmlDeviceGetClockInfo(dev, nvml.NVML_CLOCK_SM, uint_buf)
			) then
				d.clocks.sm = uint_buf[0]
			end
			if nvml_expect(
				nvml.nvmlDeviceGetClockInfo(dev, nvml.NVML_CLOCK_MEM, uint_buf)
			) then
				d.clocks.memory = uint_buf[0]
			end

			-- FAN SPEED
			if nvml_expect(
				nvml.nvmlDeviceGetFanSpeed(dev, uint_buf)
			) then
				d.fan_speed = uint_buf[0]
			end

			-- MEMORY
			if nvml_expect(
				nvml.nvmlDeviceGetMemoryInfo(dev, memory_buf)
			) then
				d.memory_info = {
					total = memory_buf[0].total,
					free = memory_buf[0].free,
					used = memory_buf[0].used
				}
			end

			-- POWER
			if nvml_expect(
				nvml.nvmlDeviceGetPowerUsage(dev, uint_buf)
			) then
				d.power_usage = uint_buf[0]
			end

			-- UTILISATION
			if nvml_expect(
				nvml.nvmlDeviceGetUtilizationRates(dev, utilisation_buf)
			) then
				d.utilisation = {
					gpu = utilisation_buf[0].gpu,
					memory = utilisation_buf[0].memory,
				}
			end
			devices[uuid] = d;
		end
	end
	return devices
end

function NVMLDataProvider:get_text()
	local status, res = pcall(function()
		local tbl = self.config.formatter_function(self, get_devices(), MarkupElement)
		assert(tbl and type(tbl) == "table", "The NVML data provider formatter function must return a table of elements!")
		tbl = remove_nils_from_array(tbl);
		for _,v in ipairs(tbl) do
			check_class(v, "MarkupElement")
		end
		return tbl
	end)
	if not status then
		log_error("NVML data provider plugin error: formatter function threw and error: " .. res)
		return {
			MarkupElement.new({
				text ="NVML data provider plugin error.\nPlease check the log.\n"
			})
		}
	end
	return res
end

function NVMLDataProvider.new(config)
	local self ={}
	setmetatable(self, NVMLDataProvider)
	self.config = config
	return self
end

return NVMLDataProvider
