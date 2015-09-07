/*
 * Copyright (C) 2013-2014 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef FONTRENDERER_HPP_
#define FONTRENDERER_HPP_

#include "Colour.hpp"
#include "VertexBuffer.hpp"
#include <fontconfig/fontconfig.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include<vector>
#include<string>
#include"Optional.hpp"

namespace glxosd {

struct Glyph {
	VertexBuffer* vertexBuffer;
	FT_Vector advance;
	FT_Glyph_Metrics metrics;
	~Glyph() {
		delete vertexBuffer;
	}
};

class FontRenderer {
public:
	FontRenderer(std::string name, int fontSize, int horizontalDPI,
			int verticalDPI, float outlineWidth);

	void render(int width, int height, std::string str);

	void setFontColour(ColourRGBA colour);
	void setFontOutlineColour(ColourRGBA colour);

	void setTextSpacingX(float spacing);
	void setTextSpacingY(float spacing);
	void setTextPositionX(int position);
	void setTextPositionY(int position);

	virtual ~FontRenderer();
private:

	FT_Library library;
	FT_Face face;
	std::vector<Optional<Glyph>*> glyphs;

	signed short int lineHeight;

	int fontSize;
	float outlineWidth;

	float textSpacingX;
	float textSpacingY;
	int textPositionX;
	int textPositionY;

	ColourRGBA fontColour;
	ColourRGBA fontOutlineColour;

	const Glyph* createGlyph(char c);
	const Glyph* getGlyph(char c);

	std::string getFontPath(const std::string& name);

};

} /* namespace glxosd */

#endif /* FONTRENDERER_HPP_ */
