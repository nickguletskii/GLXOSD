local homePath = (os.getenv ("XDG_CONFIG_HOME") or (os.getenv ("HOME").."/.config/")).."/glxosd/";
local customPath = os.getenv ("GLXOSD_ADDITIONAL_CONFIG_LOCATION");
if not customPath or #customPath == 0 then
	customPath = homePath;
end
return {
	customPath,
	homePath,
	"/etc/glxosd/",
	glxosdPackageRoot.."/glxosd/conf/"
}
