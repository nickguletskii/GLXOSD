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
#include "GLXOSD.hpp"
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
		FcChar8* file = nullptr;
		if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch)
			fontFile = std::string((char*) file);
		FcPatternDestroy(font);
	}
	FcPatternDestroy(pattern);
	return fontFile;
}
OSDInstance::OSDInstance() :
		osdText("Gathering data...") {
	ConfigurationManager* configurationManager =
			GLXOSD::instance()->getConfigurationManager();
//Font initialisation, obviously...
	std::string fontName = configurationManager->getProperty<std::string>(
			"font_name");
	int fontSize = configurationManager->getProperty<int>("font_size_int");

	std::string fontPath = findFont(fontName.c_str());
	std::cout << "[GLXOSD] Loading font \"" << fontPath << "\" with size "
			<< fontSize << "..." << std::endl;

	font = new FTGLExtrdFont(fontPath.c_str());
	font->FaceSize(fontSize);
//Trick from http://gamedev.stackexchange.com/a/46512
	font->Depth(0);
	font->Outset(0, 1);
	currentFrameCount = 0;
	framesPerSecond = 0;
//Set the time for the first time.
	struct timespec current;
	clock_gettime(CLOCK_MONOTONIC, &current);
	previousTime = ((current.tv_sec * 1000UL) + (current.tv_nsec / 1000000UL));

	glxosd_glUseProgram = (glUseProgram_type) glXGetProcAddress(
			(const GLubyte*) "glUseProgram");
}

OSDInstance::~OSDInstance() {
	delete font;
}
void OSDInstance::update(long currentMilliseconds) {
	ConfigurationManager* configurationManager =
			GLXOSD::instance()->getConfigurationManager();

	framesPerSecond = 1000.0 * currentFrameCount
			/ (currentMilliseconds - previousTime);
	previousTime = currentMilliseconds;
	currentFrameCount = 0;

	auto fpsFormat = configurationManager->getProperty<boost::format>(
			"fps_format");
	std::stringstream osdTextBuilder;
	osdTextBuilder << boost::format(fpsFormat) % framesPerSecond;

	std::vector<PluginDataProvider>* dataProviders =
			GLXOSD::instance()->getPluginDataProviders();

	std::for_each(dataProviders->begin(), dataProviders->end(),
			[&osdTextBuilder](PluginDataProvider &sensorDataProvider) {
				std::string* strPtr = sensorDataProvider(GLXOSD::instance());
				osdTextBuilder << *strPtr;
				delete strPtr; // Prevent memory leak. Goddamn C ABI forcing me to use pointers!!
				// The weird thing is: it gets deleted even without the delete. How and why?
			});

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

	if (glxosd_glUseProgram != nullptr)
		glxosd_glUseProgram(0);				//Reset shader if supported

	glPushMatrix();
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS); //Make sure that we revert the state after we do everything.
		{
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			{
				glLoadIdentity();
				glOrtho(0, width, 0, height, -1, 1);
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				{
					glLoadIdentity();

					glDisable(GL_ALPHA_TEST);
					glDisable(GL_AUTO_NORMAL);
					glDisable(GL_COLOR_LOGIC_OP);
					glDisable(GL_COLOR_TABLE);
					glDisable(GL_CONVOLUTION_1D);
					glDisable(GL_CONVOLUTION_2D);
					glDisable(GL_LINE_SMOOTH);
					glDisable(GL_MULTISAMPLE);
					glDisable(GL_CULL_FACE);
					glDisable(GL_DEPTH_TEST);
					glDisable(GL_DITHER);
					glDisable(GL_FOG);
					glDisable(GL_HISTOGRAM);
					glDisable(GL_INDEX_LOGIC_OP);
					glDisable(GL_LIGHTING);
					glDisable(GL_MINMAX);
					glDisable(GL_NORMALIZE);
					glDisable(GL_SCISSOR_TEST);
					glDisable(GL_SEPARABLE_2D);
					glDisable(GL_STENCIL_TEST);

					glDisableClientState(GL_COLOR_ARRAY);
					glDisableClientState(GL_EDGE_FLAG_ARRAY);
					glDisableClientState(GL_INDEX_ARRAY);
					glDisableClientState(GL_NORMAL_ARRAY);
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
					glDisableClientState(GL_VERTEX_ARRAY);

					glEnable(GL_BLEND);
					glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

					glRenderMode(GL_RENDER);

					glClear(GL_ALL_ATTRIB_BITS);

					int lineNumber = 0;
					std::stringstream osd_text_reader;
					osd_text_reader << osdText;

					ConfigurationManager* configurationManager =
							GLXOSD::instance()->getConfigurationManager();
					int fontColourR = configurationManager->getProperty<int>(
							"font_colour_r_int");
					int fontColourG = configurationManager->getProperty<int>(
							"font_colour_g_int");
					int fontColourB = configurationManager->getProperty<int>(
							"font_colour_b_int");
					int textPositionX = configurationManager->getProperty<int>(
							"text_pos_x_int");
					int textPositionY = configurationManager->getProperty<int>(
							"text_pos_y_int");
					int fontSize = configurationManager->getProperty<int>(
							"font_size_int");
					int textSpacingY = configurationManager->getProperty<int>(
							"text_spacing_y_int");
					int textSpacingX = configurationManager->getProperty<int>(
							"text_spacing_x_int");
					bool showTextOutline = configurationManager->getProperty<
							bool>("show_text_outline_bool");

					while (!osd_text_reader.eof()) {
						lineNumber++;
						std::string line;
						std::getline(osd_text_reader, line);

						std::transform(line.begin(), line.end(), line.begin(),
								::toupper);

//Trick from http://gamedev.stackexchange.com/a/46512
						glColor3ub(fontColourR, fontColourG, fontColourB);

						int xpos = textPositionX;
						int ypos = height - textPositionY
								- (fontSize + textSpacingY) * lineNumber;

						font->Render(line.c_str(), line.size(),
								FTPoint(xpos, ypos), FTPoint(textSpacingX, 0),
								FTGL::RENDER_FRONT);

						if (showTextOutline) {
							glColor3ub(0, 0, 0);

							font->Render(line.c_str(), line.size(),
									FTPoint(xpos, ypos),
									FTPoint(textSpacingX, 0),
									FTGL::RENDER_SIDE);
						}
					}
					glMatrixMode(GL_PROJECTION);
				}
				glPopMatrix();
				glMatrixMode(GL_MODELVIEW);
			}
			glPopMatrix();
			glClear(GL_DEPTH_BUFFER_BIT);
		}
		glPopAttrib();
	}
	glPopMatrix();
}

}

