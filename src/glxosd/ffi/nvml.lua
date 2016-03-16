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

local ffi = require("ffi")

local status, nvml = pcall(function() return ffi.load("/usr/lib/nvidia-352/libnvidia-ml.so.1") end)
if not status then
	return nil
end


ffi.cdef[[
typedef void* nvmlDevice_t;

typedef enum nvmlReturn_enum {
	NVML_SUCCESS = 0,
	NVML_ERROR_UNINITIALIZED = 1,
	NVML_ERROR_INVALID_ARGUMENT = 2,
	NVML_ERROR_NOT_SUPPORTED = 3,
	NVML_ERROR_NO_PERMISSION = 4,
	NVML_ERROR_ALREADY_INITIALIZED = 5,
	NVML_ERROR_NOT_FOUND = 6,
	NVML_ERROR_INSUFFICIENT_SIZE = 7,
	NVML_ERROR_INSUFFICIENT_POWER = 8,
	NVML_ERROR_DRIVER_NOT_LOADED = 9,
	NVML_ERROR_TIMEOUT = 10,
	NVML_ERROR_IRQ_ISSUE = 11,
	NVML_ERROR_LIBRARY_NOT_FOUND = 12,
	NVML_ERROR_FUNCTION_NOT_FOUND = 13,
	NVML_ERROR_CORRUPTED_INFOROM = 14,
	NVML_ERROR_UNKNOWN = 999
} nvmlReturn_t;

typedef enum nvmlPerfPolicyType_t_ {
	NVML_PERF_POLICY_POWER = 0,
	NVML_PERF_POLICY_THERMAL = 1,
	NVML_PERF_POLICY_COUNT
} nvmlPerfPolicyType_t;

typedef enum nvmlClockType_t_ {
	NVML_CLOCK_GRAPHICS = 0,
	NVML_CLOCK_SM = 1,
	NVML_CLOCK_MEM = 2,
	NVML_CLOCK_COUNT
} nvmlClockType_t;

typedef enum nvmlTemperatureSensors_t_ {
	NVML_TEMPERATURE_GPU = 0, NVML_TEMPERATURE_COUNT
} nvmlTemperatureSensors_t;

typedef enum nvmlTemperatureThresholds_t_ {
	NVML_TEMPERATURE_THRESHOLD_SHUTDOWN = 0,
	NVML_TEMPERATURE_THRESHOLD_SLOWDOWN = 1,
	NVML_TEMPERATURE_THRESHOLD_COUNT
} nvmlTemperatureThresholds_t;

typedef struct nvmlUtilization_t_ {
	unsigned int gpu;
	unsigned int memory;
} nvmlUtilization_t;

typedef struct nvmlViolationTime_t_ {
	unsigned long long referenceTime;
	unsigned long long violationTime;
} nvmlViolationTime_t;

typedef struct nvmlMemory_st_ {
	unsigned long long total;
	unsigned long long free;
	unsigned long long used;
} nvmlMemory_t;

nvmlReturn_t nvmlInit(void);
nvmlReturn_t nvmlDeviceGetClockInfo(nvmlDevice_t device, nvmlClockType_t type,
		unsigned int* clock);
nvmlReturn_t nvmlDeviceGetCount(unsigned int* deviceCount);
nvmlReturn_t nvmlDeviceGetFanSpeed(nvmlDevice_t device, unsigned int* speed);
nvmlReturn_t nvmlDeviceGetHandleByIndex(unsigned int index,
		nvmlDevice_t* device);
nvmlReturn_t nvmlDeviceGetMaxClockInfo(nvmlDevice_t device,
		nvmlClockType_t type, unsigned int* clock);
nvmlReturn_t nvmlDeviceGetMemoryInfo(nvmlDevice_t device, nvmlMemory_t* memory);
nvmlReturn_t nvmlDeviceGetName(nvmlDevice_t device, char* name,
		unsigned int length);
nvmlReturn_t nvmlDeviceGetPowerUsage(nvmlDevice_t device, unsigned int* power);
nvmlReturn_t nvmlDeviceGetTemperature(nvmlDevice_t device,
		nvmlTemperatureSensors_t sensorType, unsigned int* temp);
nvmlReturn_t nvmlDeviceGetTemperatureThreshold(nvmlDevice_t device,
		nvmlTemperatureThresholds_t thresholdType, unsigned int* temp);
nvmlReturn_t nvmlDeviceGetUtilizationRates(nvmlDevice_t device,
		nvmlUtilization_t* utilization);
nvmlReturn_t nvmlDeviceGetVbiosVersion(nvmlDevice_t device, char* version,
		unsigned int length);
nvmlReturn_t nvmlDeviceGetViolationStatus(nvmlDevice_t device,
		nvmlPerfPolicyType_t perfPolicyType, nvmlViolationTime_t* violTime);
nvmlReturn_t nvmlDeviceGetUUID(nvmlDevice_t device, char* uuid,
		unsigned int length);
]]

NVML_RET_LOOKUP = {
	NVML_SUCCESS = 0,
	NVML_ERROR_UNINITIALIZED = 1,
	NVML_ERROR_INVALID_ARGUMENT = 2,
	NVML_ERROR_NOT_SUPPORTED = 3,
	NVML_ERROR_NO_PERMISSION = 4,
	NVML_ERROR_ALREADY_INITIALIZED = 5,
	NVML_ERROR_NOT_FOUND = 6,
	NVML_ERROR_INSUFFICIENT_SIZE = 7,
	NVML_ERROR_INSUFFICIENT_POWER = 8,
	NVML_ERROR_DRIVER_NOT_LOADED = 9,
	NVML_ERROR_TIMEOUT = 10,
	NVML_ERROR_IRQ_ISSUE = 11,
	NVML_ERROR_LIBRARY_NOT_FOUND = 12,
	NVML_ERROR_FUNCTION_NOT_FOUND = 13,
	NVML_ERROR_CORRUPTED_INFOROM = 14,
	NVML_ERROR_UNKNOWN = 999
}

function nvml_expect(ret, exp)
	exp = exp or {
		NVML_SUCCESS =true,
		NVML_ERROR_NOT_SUPPORTED = false,
		NVML_ERROR_GPU_IS_LOST = false
	}
	ret = tonumber(ret)
	for k, v in pairs(exp) do
		if NVML_RET_LOOKUP[k]==ret then
			return v
		end
	end
	for k, v in pairs(NVML_RET_LOOKUP) do
		if v==ret then
			error("Unexpected NVML return: " .. k)
		end
	end
	error("Unknown NVML return: "..ret)
end
ffi_types["nvmlDevice_t_ref"] = ffi.typeof("nvmlDevice_t[1]")
ffi_types["nvmlMemory_t_ref"] = ffi.typeof("nvmlMemory_t[1]")
ffi_types["nvmlUtilization_t_ref"] = ffi.typeof("nvmlUtilization_t[1]")

return nvml
