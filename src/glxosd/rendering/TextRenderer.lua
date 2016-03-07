--[[
Copyright (C) 2016 Nick Guletskii

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without selftriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPselfS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
]]

function readAll(file)
	local f = io.open(file, "rb")
	local content = f:read("*all")
	f:close()
	return content
end
local FontUtil = require("util/fontutil")

local vertex_shader = readAll(resource_resolver("freetype-gl-glxosd/freetype-gl/shaders/text.vert"))
local fragment_shader = readAll(resource_resolver("freetype-gl-glxosd/freetype-gl/shaders/text.frag"))

local shader_support = require("rendering/shadersupport")
local MarkupElement = require("rendering/MarkupElement")

local OUTLINE_TYPES = {
	none = 0,
	outer = 1,
	inner = 2,
	inside = 3
}

local CONFIG_SCHEMA = {
	font="string",
	font_size="number",
	outline={
		type=function(value)
			return OUTLINE_TYPES[value]
		end,
		thickness=function(value)
			return type(value)=="number" and value>=0
		end,
		color="color"
	},
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
	background_color="color"
}

local TextRenderer = {
	}

TextRenderer.__index = TextRenderer;

function TextRenderer:build_outline_markup(normal, type, color, thickness)
	local outline = ffi_types.markup_t_ref(normal)

	if type~="none" then
		outline[0].outline=1
		outline[0].foreground_color = ffi_types.vec4(self.config.outline.color)
	end
	outline[0].font = self.font2
	return outline
end

function TextRenderer:print_text(pen, text)
	local pen1 = ffi_types.vec2(pen)
	local pen2 = ffi_types.vec2(pen)
	
	local char_count = 0
	for _, markup_element in ipairs(text) do
		char_count = char_count + #(markup_element.text)
	end
	
	if char_count == 0 then
		table.insert(text, MarkupElement.new({text="No data available!\n"}))
	end
	
	for _, markup_element in ipairs(text) do
		markup_element:check()
		local cur = ffi_types.markup_t_ref(self.normal)
		if markup_element.gamma then
			cur[0].gamma = markup_element.gamma
		end
		if markup_element.color then
			cur[0].foreground_color = ffi_types.vec4(markup_element.color)
		end
		if markup_element.background_color then
			cur[0].background_color = ffi_types.vec4(markup_element.background_color)
		end
		if markup_element.underline and markup_element.underline.enabled then
			cur[0].underline =  markup_element.underline.enabled and 1 or 0
		end
		if markup_element.underline and  markup_element.underline.color then
			cur[0].underline_color = markup_element.underline.color
		end
		if markup_element.underline and markup_element.underline.enabled then
			cur[0].underline = markup_element.underline.enabled and 1 or 0
		end
		if markup_element.overline and markup_element.overline.color then
			cur[0].overline_color = markup_element.overline.color
		end
		if markup_element.strikethrough and markup_element.strikethrough.enabled then
			cur[0].strikethrough =  markup_element.strikethrough.enabled and 1 or 0
		end
		if markup_element.strikethrough and markup_element.strikethrough.color then
			cur[0].strikethrough_color = markup_element.strikethrough.color
		end
		local cur_outline= self:build_outline_markup(cur,
			cur[0].outline_color or
			self.config.outline.color)
		freetype_gl.text_buffer_printf( self.buffer1, pen1,
			cur, ffi_types.char_array_from_string(markup_element.text), nil)
		freetype_gl.text_buffer_align( self.buffer1, pen1, freetype_gl.ALIGN_LEFT )
		freetype_gl.text_buffer_printf( self.buffer2, pen2,
			cur_outline, ffi_types.char_array_from_string(markup_element.text), nil)
		freetype_gl.text_buffer_align( self.buffer2, pen2, freetype_gl.ALIGN_LEFT )
	end
	local bounds = freetype_gl.text_buffer_get_bounds(self.buffer1, pen1)
	return bounds
end

function TextRenderer:set_text (text)
	freetype_gl.text_buffer_clear(self.buffer1)
	freetype_gl.text_buffer_clear(self.buffer2)

	local pen = ffi_types.vec2({{0, 0}})
	self.bounds = self:print_text(pen, text)

	freetype_gl.texture_atlas_upload(self.buffer1[0].manager[0].atlas )
	freetype_gl.texture_atlas_upload(self.buffer2[0].manager[0].atlas )
end

function TextRenderer:render (width, height)
	freetype_gl.mat4_set_translation(self.model, 0, height - self.bounds.y, 0)
	gl.glViewport(0, 0, width, height)

	gl.glEnable(GL_BLEND)
	gl.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)

	freetype_gl.mat4_set_orthographic(self.projection, 0, width, 0, height, -1, 1)

	gl.glUseProgram(self.shader)
	gl.glUniform1i(gl.glGetUniformLocation(self.shader, "texture"),
		0)
	gl.glUniformMatrix4fv(gl.glGetUniformLocation(self.shader, "model"),
		1, 0, self.model[0].data)
	gl.glUniformMatrix4fv(gl.glGetUniformLocation(self.shader, "view"),
		1, 0, self.view[0].data)
	gl.glUniformMatrix4fv(gl.glGetUniformLocation(self.shader, "projection"),
		1, 0, self.projection[0].data)

	freetype_gl.text_buffer_render(self.buffer1 )
	freetype_gl.text_buffer_render(self.buffer2 )

end

function TextRenderer:init(config)
	self.config = config

	self.shader = shader_support.create_shader_program(vertex_shader, fragment_shader)

	local lcd_filter_mode =  config.lcd_filter_enabled and 3 or 1

	self.buffer2 = freetype_gl.text_buffer_new_with_program(lcd_filter_mode, self.shader)
	self.buffer1 = freetype_gl.text_buffer_new_with_program(lcd_filter_mode, self.shader)


	self.font_file = FontUtil.get_font(config.font)
	self.fontFileC1 = ffi_types.char_array_from_string(self.font_file)
	self.fontFileC2 = ffi_types.char_array_from_string(self.font_file)

	self.font2  = freetype_gl.texture_font_new_from_file(self.buffer2[0].manager[0].atlas, config.font_size, self.fontFileC2)
	self.font1  = freetype_gl.texture_font_new_from_file(self.buffer1[0].manager[0].atlas, config.font_size, self.fontFileC1)
	self.font2[0].outline_type = OUTLINE_TYPES[config.outline.type]
	self.font2[0].outline_thickness = config.outline.thickness

	self.model = ffi_types.mat4_ref()
	self.view = ffi_types.mat4_ref()
	self.projection = ffi_types.mat4_ref()
	freetype_gl.mat4_set_identity(self.projection)
	freetype_gl.mat4_set_identity(self.model)
	freetype_gl.mat4_set_identity(self.view)


	local normal =  ffi_types.markup_t_ref()
	normal[0].family = fontFileC
	normal[0].size = config.font_size
	normal[0].bold = 0
	normal[0].italic = 0
	normal[0].gamma = config.gamma
	normal[0].foreground_color = ffi_types.vec4(config.color)
	normal[0].background_color = ffi_types.vec4(config.background_color)
	normal[0].underline =  config.underline.enabled and 1 or 0
	normal[0].underline_color = config.underline.color
	normal[0].overline = config.overline.enabled and 1 or 0
	normal[0].overline_color = config.overline.color
	normal[0].strikethrough =  config.strikethrough.enabled and 1 or 0
	normal[0].strikethrough_color = config.strikethrough.color
	normal[0].outline = 0
	normal[0].font = self.font1

	self.normal = normal

end

function TextRenderer.new(config)
	ConfigurationManager.check_schema(config,
		CONFIG_SCHEMA, false, "[text renderer configuration]")

	local self ={}
	setmetatable(self, TextRenderer)

	self:init(config)

	return self
end

return TextRenderer
