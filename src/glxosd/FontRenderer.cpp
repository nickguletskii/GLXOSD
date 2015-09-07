/*
 * Copyright (C) 2013-2014 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "FontRenderer.hpp"
#include <GL/gl.h>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include FT_ERRORS_H
#include FT_STROKER_H
#include FT_BITMAP_H

namespace glxosd {

void handleFreetypeError(FT_Error error) {
	if (error == 0)
		return;
	std::string errorMessage = "";

	//XXX: Find a cleaner way to translate error code to error message
	switch (error) {
#define HANDLE_FREETYPE_ERROR(x) case FT_Err_##x : {errorMessage = #x; break;}
	HANDLE_FREETYPE_ERROR(Cannot_Open_Resource)
	HANDLE_FREETYPE_ERROR(Unknown_File_Format)
	HANDLE_FREETYPE_ERROR(Invalid_File_Format)
	HANDLE_FREETYPE_ERROR(Invalid_Version)
	HANDLE_FREETYPE_ERROR(Lower_Module_Version)
	HANDLE_FREETYPE_ERROR(Invalid_Argument)
	HANDLE_FREETYPE_ERROR(Unimplemented_Feature)
	HANDLE_FREETYPE_ERROR(Invalid_Table)
	HANDLE_FREETYPE_ERROR(Invalid_Offset)
	HANDLE_FREETYPE_ERROR(Array_Too_Large)
	HANDLE_FREETYPE_ERROR(Missing_Module)
	HANDLE_FREETYPE_ERROR(Missing_Property)
	HANDLE_FREETYPE_ERROR(Invalid_Glyph_Index)
	HANDLE_FREETYPE_ERROR(Invalid_Character_Code)
	HANDLE_FREETYPE_ERROR(Invalid_Glyph_Format)
	HANDLE_FREETYPE_ERROR(Cannot_Render_Glyph)
	HANDLE_FREETYPE_ERROR(Invalid_Outline)
	HANDLE_FREETYPE_ERROR(Invalid_Composite)
	HANDLE_FREETYPE_ERROR(Too_Many_Hints)
	HANDLE_FREETYPE_ERROR(Invalid_Pixel_Size)
	HANDLE_FREETYPE_ERROR(Invalid_Handle)
	HANDLE_FREETYPE_ERROR(Invalid_Library_Handle)
	HANDLE_FREETYPE_ERROR(Invalid_Driver_Handle)
	HANDLE_FREETYPE_ERROR(Invalid_Face_Handle)
	HANDLE_FREETYPE_ERROR(Invalid_Size_Handle)
	HANDLE_FREETYPE_ERROR(Invalid_Slot_Handle)
	HANDLE_FREETYPE_ERROR(Invalid_CharMap_Handle)
	HANDLE_FREETYPE_ERROR(Invalid_Cache_Handle)
	HANDLE_FREETYPE_ERROR(Invalid_Stream_Handle)
	HANDLE_FREETYPE_ERROR(Too_Many_Drivers)
	HANDLE_FREETYPE_ERROR(Too_Many_Extensions)
	HANDLE_FREETYPE_ERROR(Out_Of_Memory)
	HANDLE_FREETYPE_ERROR(Unlisted_Object)
	HANDLE_FREETYPE_ERROR(Cannot_Open_Stream)
	HANDLE_FREETYPE_ERROR(Invalid_Stream_Seek)
	HANDLE_FREETYPE_ERROR(Invalid_Stream_Skip)
	HANDLE_FREETYPE_ERROR(Invalid_Stream_Read)
	HANDLE_FREETYPE_ERROR(Invalid_Stream_Operation)
	HANDLE_FREETYPE_ERROR(Invalid_Frame_Operation)
	HANDLE_FREETYPE_ERROR(Nested_Frame_Access)
	HANDLE_FREETYPE_ERROR(Invalid_Frame_Read)
	HANDLE_FREETYPE_ERROR(Raster_Uninitialized)
	HANDLE_FREETYPE_ERROR(Raster_Corrupted)
	HANDLE_FREETYPE_ERROR(Raster_Overflow)
	HANDLE_FREETYPE_ERROR(Raster_Negative_Height)
	HANDLE_FREETYPE_ERROR(Too_Many_Caches)
	HANDLE_FREETYPE_ERROR(Invalid_Opcode)
	HANDLE_FREETYPE_ERROR(Too_Few_Arguments)
	HANDLE_FREETYPE_ERROR(Stack_Overflow)
	HANDLE_FREETYPE_ERROR(Code_Overflow)
	HANDLE_FREETYPE_ERROR(Bad_Argument)
	HANDLE_FREETYPE_ERROR(Divide_By_Zero)
	HANDLE_FREETYPE_ERROR(Invalid_Reference)
	HANDLE_FREETYPE_ERROR(Debug_OpCode)
	HANDLE_FREETYPE_ERROR(ENDF_In_Exec_Stream)
	HANDLE_FREETYPE_ERROR(Nested_DEFS)
	HANDLE_FREETYPE_ERROR(Invalid_CodeRange)
	HANDLE_FREETYPE_ERROR(Execution_Too_Long)
	HANDLE_FREETYPE_ERROR(Too_Many_Function_Defs)
	HANDLE_FREETYPE_ERROR(Too_Many_Instruction_Defs)
	HANDLE_FREETYPE_ERROR(Table_Missing)
	HANDLE_FREETYPE_ERROR(Horiz_Header_Missing)
	HANDLE_FREETYPE_ERROR(Locations_Missing)
	HANDLE_FREETYPE_ERROR(Name_Table_Missing)
	HANDLE_FREETYPE_ERROR(CMap_Table_Missing)
	HANDLE_FREETYPE_ERROR(Hmtx_Table_Missing)
	HANDLE_FREETYPE_ERROR(Post_Table_Missing)
	HANDLE_FREETYPE_ERROR(Invalid_Horiz_Metrics)
	HANDLE_FREETYPE_ERROR(Invalid_CharMap_Format)
	HANDLE_FREETYPE_ERROR(Invalid_PPem)
	HANDLE_FREETYPE_ERROR(Invalid_Vert_Metrics)
	HANDLE_FREETYPE_ERROR(Could_Not_Find_Context)
	HANDLE_FREETYPE_ERROR(Invalid_Post_Table_Format)
	HANDLE_FREETYPE_ERROR(Invalid_Post_Table)
	HANDLE_FREETYPE_ERROR(Syntax_Error)
	HANDLE_FREETYPE_ERROR(Stack_Underflow)
	HANDLE_FREETYPE_ERROR(Ignore)
	HANDLE_FREETYPE_ERROR(No_Unicode_Glyph_Name)
	HANDLE_FREETYPE_ERROR(Glyph_Too_Big)
	HANDLE_FREETYPE_ERROR(Missing_Startfont_Field)
	HANDLE_FREETYPE_ERROR(Missing_Font_Field)
	HANDLE_FREETYPE_ERROR(Missing_Size_Field)
	HANDLE_FREETYPE_ERROR(Missing_Fontboundingbox_Field)
	HANDLE_FREETYPE_ERROR(Missing_Chars_Field)
	HANDLE_FREETYPE_ERROR(Missing_Startchar_Field)
	HANDLE_FREETYPE_ERROR(Missing_Encoding_Field)
	HANDLE_FREETYPE_ERROR(Missing_Bbx_Field)
	HANDLE_FREETYPE_ERROR(Bbx_Too_Big)
	HANDLE_FREETYPE_ERROR(Corrupted_Font_Header)
	HANDLE_FREETYPE_ERROR(Corrupted_Font_Glyphs)
#undef HANDLE_FREETYPE_ERROR
	}
	std::replace(errorMessage.begin(), errorMessage.end(), '_', ' ');
	throw std::runtime_error("Freetype error: " + errorMessage);
}

FontRenderer::FontRenderer(std::string name, int fontSize, int horizontalDPI,
		int verticalDPI, float outlineWidth) :
		fontSize(fontSize), outlineWidth(outlineWidth),
		textSpacingX(0),
		textSpacingY(0),
		textPositionX(0),
		textPositionY(0) {
	std::string fontFile = getFontPath(name);

	if (fontFile.empty())
		throw std::runtime_error("Couldn't find font: " + name);

	FT_Error error = FT_Init_FreeType(&library);
	handleFreetypeError(error);

	error = FT_New_Face(library, fontFile.c_str(), 0, &face);
	handleFreetypeError(error);

	error = FT_Set_Char_Size(face, 0, fontSize * 64, horizontalDPI,
			verticalDPI);
	handleFreetypeError(error);

	glyphs = std::vector<Optional<Glyph>*>(256, nullptr);

	lineHeight = FT_MulFix((face->bbox.yMax - face->bbox.yMin),
			face->size->metrics.y_scale);
}

void FontRenderer::setTextSpacingX(float spacing) {
	textSpacingX = spacing;
}

void FontRenderer::setTextSpacingY(float spacing) {
	textSpacingY = spacing;
}

void FontRenderer::setTextPositionX(int position) {
	textPositionX = position;
}

void FontRenderer::setTextPositionY(int position) {
	textPositionY = position;
}

std::string FontRenderer::getFontPath(const std::string& name) {
	FcConfig* fontConfig = FcInitLoadConfigAndFonts();
	FcPattern* pattern = FcNameParse((const FcChar8*) ((name.c_str())));
	FcConfigSubstitute(fontConfig, pattern, FcMatchPattern);
	FcDefaultSubstitute(pattern);
	FcResult result;
	std::string fontFile = "";
	FcPattern* font = FcFontMatch(fontConfig, pattern, &result);
	if (font) {
		FcChar8* file = nullptr;
		if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch)
			fontFile = std::string((char*) file);
		FcPatternDestroy(font);
	}
	FcPatternDestroy(pattern);
	FcConfigDestroy(fontConfig);
	FcFini();
	return fontFile;
}

void FontRenderer::render(int width, int height, std::string str) {
	int x = (textPositionX * 64);
	int y = -(textPositionY * 64) - lineHeight;
	for (auto c : str) {
		if (c == '\n') {
			x = textPositionX * 64;
			y -= lineHeight + (int) (textSpacingY * 64);
			continue;
		}

		const Glyph* glyph = getGlyph(c);
		if (glyph == nullptr) // If the glyph doesn't exist in the font, ignore it
			continue;

		GLfloat screenX = ((GLfloat) (x + glyph->metrics.horiBearingX)) / 64.0f;
		GLfloat screeny = ((GLfloat) (y + glyph->metrics.horiBearingY
				- glyph->metrics.height)) / 64.0f;

		if (glyph->vertexBuffer != nullptr) // If the glyph is empty (due to an error or something else), don't render it
			glyph->vertexBuffer->render(screenX, screeny, width, height);

		x += glyph->advance.x + (int) (textSpacingX * 64);
		y -= glyph->advance.y;
	}
}

const Glyph* FontRenderer::getGlyph(char c) {
	if (glyphs[c] == nullptr) {
		glyphs[c] = new Optional<Glyph>(createGlyph(c));
	}
	return glyphs[c]->get();
}

const Glyph* FontRenderer::createGlyph(char c) {
	FT_UInt glyphIndex = FT_Get_Char_Index(face, c);

	if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_RENDER))
		return nullptr; // If we can't load a glyph, we leave it as null so that it is ignored

	if (face->glyph->bitmap.width <= 0 || face->glyph->bitmap.rows <= 0)
		return new Glyph { nullptr, face->glyph->advance, face->glyph->metrics }; // If the glyph is empty, don't initialise its vertex buffer

	FT_Bitmap mainBitmap;
	FT_Bitmap_New(&mainBitmap);
	FT_Bitmap_Copy(library, &face->glyph->bitmap, &mainBitmap);

	if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_NO_BITMAP)) {
		FT_Bitmap_Done(library, &mainBitmap);
		return nullptr;
	}

	FT_Glyph outlineGlyph;
	FT_Stroker outlineStroker;
	FT_Stroker_New(library, &outlineStroker);
	FT_Stroker_Set(outlineStroker, (int) (outlineWidth * 64),
			FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);

	FT_Get_Glyph(face->glyph, &outlineGlyph);

	FT_Glyph_Stroke(&outlineGlyph, outlineStroker, true);
	FT_Stroker_Done(outlineStroker);

	FT_Glyph_To_Bitmap(&outlineGlyph, FT_RENDER_MODE_NORMAL, 0, true);
	FT_BitmapGlyph outlineBitmapGlyph = (FT_BitmapGlyph) outlineGlyph;

	int imageHeight = outlineBitmapGlyph->bitmap.rows;
	int imageWidth = outlineBitmapGlyph->bitmap.width;
	if (imageHeight <= 0 || imageWidth <= 0) {
		FT_Done_Glyph(outlineGlyph);
		FT_Bitmap_Done(library, &mainBitmap);
		return nullptr;
	}

	std::vector<ColourRGBA> finalColor(imageWidth * imageHeight, ColourRGBA());
	for (int outlineY = 0; outlineY < imageHeight; outlineY++) {
		for (int outlineX = 0; outlineX < imageWidth; outlineX++) {
			int y = outlineY - (imageHeight - mainBitmap.rows) / 2;
			int x = outlineX - (imageWidth - mainBitmap.width) / 2;

			GLuint baseAlpha =
					(y >= 0 && x >= 0 && y < mainBitmap.rows
							&& x < mainBitmap.width) ?
							mainBitmap.buffer[x + mainBitmap.width * y] : 0;

			GLuint outlineAlpha = outlineBitmapGlyph->bitmap.buffer[outlineX
					+ imageWidth * outlineY];

			finalColor[outlineY * imageWidth + outlineX] = (ColourRGBA(255, 255,
					255, baseAlpha) * fontColour)
					+ (ColourRGBA(255, 255, 255, outlineAlpha)
							* fontOutlineColour);

		}
	}

	std::vector<GLfloat> vertices( {
	/*-----------------------------------------*/
	/*|*/0, 0, 0, 0, 1, /* Top left corner    |*/
	/*|*/0, 1, 0, 0, 0, /* Bottom left corner |*/
	/*|*/1, 1, 0, 1, 0, /* Bottom right corner|*/
	/*|*/1, 0, 0, 1, 1, /* Top right corner   |*/
	/*-----------------------------------------*/
	});

	std::vector<GLuint> indices( {
	/*----------------------------------------------------*/
	/*|*/0, 1, 2, /* Top left, bottom left, bottom right |*/
	/*|*/2, 3, 0, /* Bottom right, top right, top left   |*/
	/*----------------------------------------------------*/
	});

	VertexBuffer* vertexBuffer = new VertexBuffer(vertices, indices, imageWidth,
			imageHeight, finalColor,
			GL_TRIANGLES);

	FT_Done_Glyph(outlineGlyph);
	FT_Bitmap_Done(library, &mainBitmap);

	return new Glyph { vertexBuffer, face->glyph->advance, face->glyph->metrics };
}

void FontRenderer::setFontColour(ColourRGBA colour) {
	fontColour = colour;
}

void FontRenderer::setFontOutlineColour(ColourRGBA colour) {
	fontOutlineColour = colour;
}

FontRenderer::~FontRenderer() {
	for (Optional<Glyph>* &glyph : glyphs) {
		delete glyph;
	}

	FT_Done_FreeType(library);
}

} /* namespace glxosd */
