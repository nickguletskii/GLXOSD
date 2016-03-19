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

local config_roots =  { glxosdPackageRoot.."/glxosd/conf/", "~/", "/etc/glxosd/"}

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

local bad_tables = {};

function validation_error(table, schema, message)
	if not bad_tables[schema] then
		bad_tables[schema] = {}
	end
	bad_tables[schema][table] = message

	glxosd_configuration_error = true
	log_error(message)
end

function ConfigurationManager.validator(func, message)
	return function(value)
		local status, err = pcall(func, value);
		if not status then
			return false, err;
		end
		if err then
			return true, nil;
		end
		return false, message;
	end
end


COLOR_VALUE_VALIDATOR = ConfigurationManager.validator(function(value)
	if type(value)~="number" then
		return false
	end
	return 0<=value and value<=1
end, "A color component must be a number between 0.0 and 1.0")

function ConfigurationManager.check_schema(table, schema, ignore_nil, context)
	if bad_tables[schema] and bad_tables[schema][table] then
		error(string.format("Previous validation of a configuration object failed."))
	end

	for k, v in pairs(schema) do
		if not ignore_nil and table[k] == nil then
			validation_error(table, schema, string.format("The field %s in %s must not be nil.", k, context))
		end
		if table[k]~=nil then
			if type(v)=="table" then
				if type(table[k]) ~= "table" then
					validation_error(table, schema, string.format("The field %s in %s must be a table, but is a %s instead.", k, context, type(table[k])))
				end
				ConfigurationManager.check_schema(table[k], v, ignore_nil, context.."."..k)
			end
			if type(v)=="function" then
				local result, err =  v(table[k], context..".".. k);
				if not result then
					validation_error(table, schema, string.format("The field %s in %s didn't pass validation: %s", k, context, err))
				end
			end
			if type(v)=="string" then
				if v=="color" then
					ConfigurationManager.check_schema(table[k], {
						r=COLOR_VALUE_VALIDATOR,
						g=COLOR_VALUE_VALIDATOR,
						b=COLOR_VALUE_VALIDATOR,
						a=COLOR_VALUE_VALIDATOR
					}, false, context.."."..k)
				elseif v=="key_combo" then
					if table[k] then
						ConfigurationManager.check_schema(table[k], {
							main_key="string",
							modifiers=ConfigurationManager.validator(function(value)
								if type(value)~="table" then
									return false
								end
								local accepted = {shift=true, alt=true, caps=true, control=true}
								for _, v in ipairs(value) do
									if not accepted[v] then
										return false
									end
								end
								return true
							end, "A key combo's modifier list must be an array that consists of zero or more of the following strings: shift, alt, caps, control")
						}, false, context.."."..k)
					end
				else
					if type(table[k])~=v then
						validation_error(table, schema, string.format("The field %s in %s must be a %s, but is a %s instead.", k, context, v, type(table[k])))
					end
				end
			end
		end
	end
end
