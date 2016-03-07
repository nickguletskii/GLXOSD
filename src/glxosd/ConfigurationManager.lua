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

local config_roots = {"/etc/glxosd/", "~/", glxosdPackageRoot.."/glxosd/conf/"}

ConfigurationManager = {}
function ConfigurationManager.file_exists(name)
	local f=io.open(name,"r")
	if f~=nil then io.close(f) return true else return false end
end

function ConfigurationManager.config_file(name)
	for k,v in pairs(config_roots) do
		if ConfigurationManager.file_exists(v .. name) then
			local loaded_chunk = assert(loadfile(v..name))
			return loaded_chunk()
		end
	end
	log_error("Couldn't find config file: "..name)
	error("Couldn't find config file.")
end

function ConfigurationManager.check_schema(table, schema, ignore_nil, context)
	for k, v in pairs(schema) do
		if not ignore_nil and table[k] == nil then
			error(string.format("The field %s in %s must not be nil.", k, context))
		end
		if table[k]~=nil then
			if type(v)=="table" then
				if type(table[k]) ~= "table" then
					error(string.format("The field %s in %s must be a table, but is a %s instead.", k, context, type(table[k])))
				end
				ConfigurationManager.check_schema(table[k], v, ignore_nil, context.."."..k)
			end
			if type(v)=="function" then
				if not v(table[k], context..".".. k) then
					error(string.format("The field %s in %s didn't pass validation.", k, context))
				end
			end
			if type(v)=="string" then
				if v=="color" then
					if type(table[k]) ~= "table" then
						error(string.format("The field %s in %s must be a color table, but is a %s instead.", k, context, type(table[k])))
					end
					local accepted = {r=0, g=0, b=0, a=0}
					for sk, _ in ipairs(accepted) do
						if not table[k][sk] then
							error(string.format("The field %s in %s must be a color table, but it doesn't have the '%s' key", k, context, sk))
						end
					end
					for sk, _ in ipairs(table[k]) do
						if not accepted[sk] then
							error(string.format("The field %s in %s must be a color table, but it has an extra '%s' key", k, context, sk))
						end
					end
				else
					if type(table[k])~=v then
						error(string.format("The field %s in %s must be a %s, but is a %s instead.", k, context, v, type(table[k])))
					end
				end
			end
		end
	end
end
