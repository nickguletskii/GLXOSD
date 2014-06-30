/*
 * Copyright (C) 2013-2014 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "Colour.hpp"
#include <algorithm>

ColourRGBA operator+(ColourRGBA left, ColourRGBA right) {
	ColourRGBA out;

	GLuint leftR = left.r;
	GLuint leftG = left.g;
	GLuint leftB = left.b;
	GLuint leftA = left.a;

	GLuint rightR = right.r;
	GLuint rightG = right.g;
	GLuint rightB = right.b;
	GLuint rightA = right.a;

	out.a = std::min<GLuint>(rightA + leftA, 255);
	if (rightA + leftA > 0) {
		out.r = leftR + ((rightR - leftR) * rightA) / 255;
		out.g = leftG + ((rightG - leftG) * rightA) / 255;
		out.b = leftB + ((rightB - leftB) * rightA) / 255;
	}
	return out;
}
ColourRGBA operator*(ColourRGBA left, ColourRGBA right) {
	ColourRGBA out;

	GLuint leftR = left.r;
	GLuint leftG = left.g;
	GLuint leftB = left.b;
	GLuint leftA = left.a;

	GLuint rightR = right.r;
	GLuint rightG = right.g;
	GLuint rightB = right.b;
	GLuint rightA = right.a;

	out.a = (leftA * rightA) / 255;
	out.r = (leftR * rightR) / 255;
	out.g = (leftG * rightG) / 255;
	out.b = (leftB * rightB) / 255;
	return out;
}
