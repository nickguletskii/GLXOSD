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
local ffi = require("ffi")
local io = require("io")

function readAll(file)
	local f = io.open(file, "rb")
	local content = f:read("*all")
	f:close()
	return content
end
local FontUtil = require("util/fontutil")

local vertex_shader = readAll(resource_resolver("freetype-gl-glxosd/freetype-gl/shaders/text.vert"))
local fragment_shader = readAll(resource_resolver("freetype-gl-glxosd/freetype-gl/shaders/text.frag"))

local shader_support = require("rendering/shader_support")
local MarkupElement = require("rendering/MarkupElement")

local OUTLINE_TYPES = {
	none = 0,
	outer = 1,
	inner = 2,
	inside = 3
}

local ALIGNMENT_MAP = {
	left= freetype_gl.ALIGN_LEFT,
	center= freetype_gl.ALIGN_CENTER,
	centre= freetype_gl.ALIGN_CENTER,
	right= freetype_gl.ALIGN_RIGHT
}

local TextRenderer = {
	}

TextRenderer.__index = TextRenderer;

TextRenderer.CONFIG_SCHEMA = {
	refresh_time = ConfigurationManager.validator(function(value)
			return type(value)=="number" and value>=0
		end, "refresh_time should be a non-negative number"),

	align_to_h = ConfigurationManager.validator(function(value)
		return value=="left" or value=="right_absolute" or value=="right"
	end, "align_to_h should be set to one of the following values: left, right, right_absolute"),

	align_to_v = ConfigurationManager.validator(function(value)
		return value=="top" or value=="bottom_absolute" or value=="bottom"
	end, "align_to_v should be set to one of the following values: top, bottom, bottom_absolute"),

	offset_x = "number",
	
	offset_y = "number",
	
	font="string",

	font_size="number",

	outline={
		type=ConfigurationManager.validator(function(value)
			return OUTLINE_TYPES[value]
		end, "outline type should be set to one of the following values: none, outer, inner, inside"),

		thickness=ConfigurationManager.validator(function(value)
			return type(value)=="number" and value>=0
		end, "outline thickness should be a non-negative number"),
	
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
	
	background_color="color",
	
	toggle_key_combo="key_combo"
}

function TextRenderer:build_outline_markup(normal, type, color, thickness, font)
	local outline = ffi_types.markup_t_ref(normal)

	if type~="none" then
		outline[0].outline=1
		outline[0].foreground_color = ffi_types.vec4(self.config.outline.color)
	end
	outline[0].font = font.outline
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
		if markup_element.font_size then
			cur[0].size = markup_element.font_size
		end
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
		if markup_element.overline and markup_element.overline.enabled then
			cur[0].overline = markup_element.overline.enabled and 1 or 0
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
		local font_family = markup_element.font or self.config.font
		local font_size = markup_element.font_size or self.config.font_size
		local font = self.font_cache[{
			font_family=font_family,
			font_size=font_size
		}]
		cur[0].font = font.main
		local cur_outline= self:build_outline_markup(cur,
			self.config.outline.type,
			cur[0].outline_color or
			self.config.outline.color,
			self.config.outline.thickness,
			font)
		freetype_gl.text_buffer_printf( self.buffer1, pen1,
			cur, ffi_types.char_array_from_string(markup_element.text), nil)
		freetype_gl.text_buffer_printf( self.buffer2, pen2,
			cur_outline, ffi_types.char_array_from_string(markup_element.text), nil)
	end
	freetype_gl.text_buffer_align( self.buffer1, pen1, self.text_alignment)
	freetype_gl.text_buffer_align( self.buffer2, pen2, self.text_alignment )
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
	local x = 0
	if self.config.align_to_h == "left" then
		x = self.config.offset_x;
	elseif self.config.align_to_h == "right" then
		x = width + self.config.offset_x - self.bounds.width;
	else
		x = width + self.config.offset_x;
	end
	local y = 0
	if self.config.align_to_v == "top" then
		y = height - self.bounds.y - self.config.offset_y;
	elseif self.config.align_to_v == "bottom" then
		y = -self.config.offset_y + self.bounds.height;
	else
		y = -self.config.offset_y;
	end
	freetype_gl.mat4_set_translation(self.model, x, y, 0)
	gl.glViewport(0, 0, ffi.cast(ffi_types.GLsizei, width), ffi.cast(ffi_types.GLsizei, height))

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

function TextRenderer:setup_matrices()
	self.model = ffi_types.mat4_ref()
	self.view = ffi_types.mat4_ref()
	self.projection = ffi_types.mat4_ref()
	freetype_gl.mat4_set_identity(self.projection)
	freetype_gl.mat4_set_identity(self.model)
	freetype_gl.mat4_set_identity(self.view)
end


function TextRenderer:init(config)
	self.config = config

	local s = self
	self.font_cache = setmetatable({},
		{
			__index=function(self, font_request)
				local key = font_request.font_family.."#"..font_request.font_size
				if(rawget(self, key) == nil) then
					local font_file = ffi_types.char_array_from_string(FontUtil.get_font(font_request.font_family))

					local new = {
						main = freetype_gl.texture_font_new_from_file(s.buffer1[0].manager[0].atlas, font_request.font_size, font_file),
						outline =freetype_gl.texture_font_new_from_file(s.buffer2[0].manager[0].atlas, font_request.font_size, font_file),
					}

					new.outline[0].outline_type = OUTLINE_TYPES[s.config.outline.type]
					new.outline[0].outline_thickness = s.config.outline.thickness

					rawset(self, key, new)
				end
				return rawget(self, key);
			end
		})

	self:setup_matrices()

	self.shader = shader_support.create_shader_program(vertex_shader, fragment_shader)

	local lcd_filter_mode =  config.lcd_filter_enabled and 3 or 1

	self.buffer2 = freetype_gl.text_buffer_new_with_program(lcd_filter_mode, self.shader)
	self.buffer1 = freetype_gl.text_buffer_new_with_program(lcd_filter_mode, self.shader)

	self.default_fonts = self.font_cache[{
		font_family=config.font,
		font_size=config.font_size
	}]

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
	normal[0].font = self.default_fonts.main

	self.normal = normal

	self.text_alignment = ALIGNMENT_MAP[self.config.text_alignment]
end

function TextRenderer:destroy()
	for k, v in pairs(self.font_cache) do
		freetype_gl.texture_font_delete(v.main)
		freetype_gl.texture_font_delete(v.outline)
	end
	freetype_gl.text_buffer_delete(self.buffer1)
	freetype_gl.text_buffer_delete(self.buffer2)
end

function TextRenderer.new(config)

	local self = {}
	setmetatable(self, TextRenderer)

	self:init(config)

	return self
end

return TextRenderer
