return {
	(os.getenv ("XDG_CONFIG_HOME") or (os.getenv ("HOME").."/.config/")).."/glxosd/",
	"/etc/glxosd/",
	glxosdPackageRoot.."/glxosd/conf/"
}