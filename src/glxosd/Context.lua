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

local Context = {
	}
Context.__index = Context;

function Context:each_plugin(func)
	for i, x in ipairs(self.plugins) do
		if x~=nil then
			local status, err = pcall(function() func(x) end)
			if not status then
				log_error("Error while exectuing plugin: ".. err)
			end
		end
	end
end

function Context:begin_frame()
	self:each_plugin(function(plugin)
		plugin:begin_frame()
	end)
end

function Context:end_frame()
	self:each_plugin(function(plugin)
		plugin:end_frame()
	end)
end

function Context:render(width, height)
	self:each_plugin(function(plugin)
		plugin:render(width, height)
	end)
end

function Context:has_keyboard_combo(key, modifiers)
	for _, plugin in ipairs(self.plugins) do
		if plugin:has_keyboard_combo(key, modifiers) then
			return true
		end
	end
	return false
end
function Context:handle_key_combo(key, modifiers)
	self:each_plugin(function(plugin)
		plugin:handle_key_combo(key, modifiers)
	end)
end

function Context.new()
	local self ={}
	setmetatable(self, Context)

	self.plugins = {}

	assert(GLXOSD_CONFIG.plugins, "The GLXOSD config must specify a list of plugins.")

	for i, plugin in pairs(GLXOSD_CONFIG.plugins) do
		if plugin ~= nil and plugin.enabled then
			table.insert(
				self.plugins,
				require(plugin.path)
					.new(plugin.config, self))
		end
	end
	return self
end

return Context;
