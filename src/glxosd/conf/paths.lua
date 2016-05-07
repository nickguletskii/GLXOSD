local io = require("io")
local ffi = require("ffi")
local arch = ConfigurationManager.config_file("arch.lua")

local function get_nvidia_driver_version()
	local f = io.open("/proc/driver/nvidia/version", "r")
	if not f then return nil end
	local res = f:read("*line"):match("Kernel Module%s+([0-9]+)%.")
	f:close()
	return res
end

local NVIDIA_DRIVER_VERSION = get_nvidia_driver_version() or "unknown"

PATH_CONFIG = {
	x64 = {
		x64 = {
			libGL = "libGL.so.1",
			libX11 = "libX11.so.6",
			libfreetype_gl = "libglxosd-freetype-gl.so.3",
			libfontconfig = "libfontconfig.so.1",
			libsensors = "libsensors.so.4",
			libnvidia_ml = "/usr/lib/nvidia-"..NVIDIA_DRIVER_VERSION.."/libnvidia-ml.so.1"
		},
		x86 = {
			libGL = "libGL.so.1",
			libX11 = "libX11.so.6",
			libfreetype_gl = "libglxosd-freetype-gl.so.3",
			libfontconfig = "libfontconfig.so.1",
			libsensors = "libsensors.so.4",
			libnvidia_ml = "/usr/lib32/nvidia-"..NVIDIA_DRIVER_VERSION.."/libnvidia-ml.so.1"
		}
	},
	x86 = {
		x86 = {
			libGL = "libGL.so.1",
			libX11 = "libX11.so.6",
			libfreetype_gl = "libglxosd-freetype-gl.so.3",
			libfontconfig = "libfontconfig.so.1",
			libsensors = "libsensors.so.4",
			libnvidia_ml = "/usr/lib/nvidia-"..NVIDIA_DRIVER_VERSION.."/libnvidia-ml.so.1"
		}
	}
}

CURRENT_PATH_CONFIG = PATH_CONFIG[arch.kernel_architecture][ffi.arch]
