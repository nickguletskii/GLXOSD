/*
 * Copyright (C) 2013 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "OSDInstance.hpp"
#include "ConfigurationManager.hpp"
#include "SensorDataProviderManager.hpp"
#include <sys/time.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdarg>
#include <iomanip>
#include <algorithm>
#include <string>
#include <fontconfig/fontconfig.h>
#include <boost/lexical_cast.hpp>
#include <boost/any.hpp>
namespace glxosd {

const std::string findFont(const char* name) {
	std::string fontFile;
	FcConfig* config = FcInitLoadConfigAndFonts();
	FcPattern* pattern = FcNameParse((const FcChar8*) (name));
	FcConfigSubstitute(config, pattern, FcMatchPattern);
	FcDefaultSubstitute(pattern);
	FcResult result;
	FcPattern* font = FcFontMatch(config, pattern, &result);
	if (font) {
		FcChar8* file = NULL;
		if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch)
			fontFile = std::string((char*) file);
		FcPatternDestroy(font);
	}
	FcPatternDestroy(pattern);
	return fontFile;
}
OSDInstance::OSDInstance() {
	configuration = glxosd::readConfigChain();
//Font initialisation, obviously...
	std::string font_path = findFont(
			getProperty<std::string>("font_name").c_str());
	std::cout << "[GLXOSD]: Loading font \"" << font_path << "\" with size "
			<< getProperty<int>("font_size_int") << "..." << std::endl;
	font = new FTGLExtrdFont(font_path.c_str());
	font->FaceSize(getProperty<int>("font_size_int"));
//Trick from http://gamedev.stackexchange.com/a/46512
	font->Depth(0);
	font->Outset(0, 1);
	currentFrameCount = 0;
	framesPerSecond = 0;
//Set the time for the first time.
	struct timespec current;
	clock_gettime(CLOCK_MONOTONIC, &current);
	previousTime = ((current.tv_sec * 1000UL) + (current.tv_nsec / 1000000UL));
}

OSDInstance::~OSDInstance() {
	delete font;
}
void OSDInstance::update(long currentMilliseconds) {
	framesPerSecond = 1000.0 * currentFrameCount
			/ (currentMilliseconds - previousTime);
	previousTime = currentMilliseconds;
	currentFrameCount = 0;
	std::stringstream osdTextBuilder;
	osdTextBuilder
			<< boost::format(getProperty<boost::format>("fps_format"))
					% framesPerSecond;
	for (size_t i = 0; i < glxosd::getSensorDataProviders().size(); i++) {
		osdTextBuilder
				<< glxosd::getSensorDataProviders()[i]->getSensorsInfo(this);
	}
	osdText = osdTextBuilder.str();
}
void OSDInstance::render(unsigned int width, unsigned int height) {
	currentFrameCount++;
	struct timespec current;
	clock_gettime(CLOCK_MONOTONIC, &current);
	long currentTimeMilliseconds = ((current.tv_sec * 1000UL)
			+ (current.tv_nsec / 1000000UL));
	//Refresh the info every 500 milliseconds
	if (currentTimeMilliseconds - previousTime >= 500) {
		update(currentTimeMilliseconds);
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
	int lineNumber = 0;
	std::stringstream osd_text_reader;
	osd_text_reader << osdText;
	while (!osd_text_reader.eof()) {
		lineNumber++;
		std::string line;
		std::getline(osd_text_reader, line);
		std::transform(line.begin(), line.end(), line.begin(), ::toupper);
//Trick from http://gamedev.stackexchange.com/a/46512
		glColor3ub(getProperty<int>("font_colour_r_int"),
				getProperty<int>("font_colour_g_int"),
				getProperty<int>("font_colour_b_int"));
		int xpos = getProperty<int>("text_pos_x_int");
		int ypos = height - getProperty<int>("text_pos_y_int")
				- (getProperty<int>("font_size_int")
						+ getProperty<int>("text_spacing_y_int")) * lineNumber;
		font->Render(line.c_str(), line.size(), FTPoint(xpos, ypos),
				FTPoint(getProperty<int>("text_spacing_x_int"), 0),
				FTGL::RENDER_FRONT);
		if (getProperty<bool>("show_text_outline_bool")) {
			glColor3ub(0, 0, 0);
			font->Render(line.c_str(), line.size(), FTPoint(xpos, ypos),
					FTPoint(getProperty<int>("text_spacing_x_int"), 0),
					FTGL::RENDER_SIDE);
		}
	}
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glClear(GL_DEPTH_BUFFER_BIT);
	glPopAttrib();
	glPopMatrix();
}

}

