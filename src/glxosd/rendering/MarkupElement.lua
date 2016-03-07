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

local MarkupElement = {
	}
MarkupElement.__index = MarkupElement;
MarkupElement.__class_name = "MarkupElement"
local SCHEMA = {
	outline_color="color",
	gamma="number",
	lcd_filter_enabled="boolean",
	underline={
		enabled="boolean",
		color="color"
	},
	overline={
		enabled="boolean",
		color="color"
	},
	strikethrough={
		enabled="boolean",
		color="color"
	},
	color="color",
	background_color="color",
	text="string"
}

function MarkupElement:check()
	ConfigurationManager.check_schema(self,SCHEMA,true, "MarkupElement")
end

function MarkupElement.new(data)
	local self ={}
	if data then
        for k, v in pairs(data) do
            self[k] = v
        end
	end
	setmetatable(self, MarkupElement)
	return self
end

MarkupElement.newline = MarkupElement.new({text="\n"})

return MarkupElement
