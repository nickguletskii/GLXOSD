/*
 * Copyright (C) 2013 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "osdinstance.hpp"
#include <sys/time.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdarg>
#include <iomanip>
#include <fontconfig/fontconfig.h>
const char* find_font(const char* name) {
	char* font_file;
	FcConfig* config = FcInitLoadConfigAndFonts();
	FcPattern* pattern = FcNameParse((const FcChar8*) (name));
	FcConfigSubstitute(config, pattern, FcMatchPattern);
	FcDefaultSubstitute(pattern);
	FcPattern* font = FcFontMatch(config, pattern, NULL);
	if (font) {
		FcChar8* file = NULL;
		if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch)
			font_file = (char*) file;
		FcPatternDestroy(font);
	}
	FcPatternDestroy(pattern);
	return font_file;
}
osd_instance::osd_instance() {
	//Font initialisation, obviously...
	font = new FTGLExtrdFont(find_font("SquareFont"));
	font->FaceSize(24);
	//Trick from http://gamedev.stackexchange.com/a/46512
	font->Depth(0);
	font->Outset(0, 1);
	current_frame_count = 0;
	frames_per_second = 0;
	//Set the time for the first time.
	struct timespec current;
	clock_gettime(CLOCK_MONOTONIC, &current);
	previous_time = ((current.tv_sec * 1000UL) + (current.tv_nsec / 1000000UL));
}

osd_instance::~osd_instance() {
	delete font;
}

void osd_instance::render(unsigned int width, unsigned int height) {
	current_frame_count++;
	struct timespec current;
	clock_gettime(CLOCK_MONOTONIC, &current);
	long current_milliseconds = ((current.tv_sec * 1000UL)
			+ (current.tv_nsec / 1000000UL));
	//Recalculate the FPS every 500 milliseconds
	if (current_milliseconds - previous_time >= 500) {
		frames_per_second = 1000.0 * current_frame_count
				/ (current_milliseconds - previous_time);
		previous_time = current_milliseconds;
		current_frame_count = 0;
	}
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS); //Make sure that we revert the state after we do everything.
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 100);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT);
	std::stringstream string_builder;
	string_builder << "FPS: " << std::fixed << std::setprecision(1)
			<< frames_per_second;
	std::string osd_text = string_builder.str();
	//Trick from http://gamedev.stackexchange.com/a/46512
	glColor3f(1.0, 0.0, 1.0);
	font->Render(osd_text.c_str(), osd_text.size(), FTPoint(4, height - 20, 0),
			FTPoint(), FTGL::RENDER_FRONT);
	glColor3f(0.0, 0.0, 0.0);
	font->Render(osd_text.c_str(), osd_text.size(), FTPoint(4, height - 20, 0),
			FTPoint(), FTGL::RENDER_SIDE);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glClear(GL_DEPTH_BUFFER_BIT);
	glPopAttrib();
	glPopMatrix();
}
