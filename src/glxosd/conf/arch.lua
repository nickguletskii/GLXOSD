local ffi = require("ffi")

ffi.cdef[[
struct utsname {
	char sysname[65];  
	char nodename[65];
	char release[65];
	char version[65];
	char machine[65];
	char domainname[65];
};
int uname(struct utsname *buf);
]]

local function get_kernel_architecture()
	local utsname = ffi.new("struct utsname[1]")
	ffi.C.uname(utsname)
	local architecture = ffi.string(utsname[0].machine)
	if architecture == "x86_64" then
		return "x64"
	elseif architecture:find("i[3-9]86") then
		return "x86"
	else
		error("Unknown system architecture! GLXOSD is not supported.")
	end
end

return {
	kernel_architecture = get_kernel_architecture()
}
