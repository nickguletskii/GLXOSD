/*
 * Copyright (C) 2013-2014 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "OSDInstance.hpp"
#include "Colour.hpp"
#include "ConfigurationManager.hpp"
#include "FontRenderer.hpp"
#include "GLAttribState.hpp"
#include "GLLoader.hpp"
#include "GLXOSD.hpp"
#include "Utils.hpp"
#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <GL/gl.h>
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>
#include <sys/time.h>
namespace glxosd {

OSDInstance::OSDInstance() :
		osdText("Gathering data...") {
	const ConfigurationManager &configurationManager =
			GLXOSD::instance()->getConfigurationManager();

	std::string fontName = configurationManager.getProperty<std::string>(
			"font_name");
	int fontSize = configurationManager.getProperty<int>("font_size_int");
	int fontColourR = configurationManager.getProperty<int>(
			"font_colour_r_int");
	int fontColourG = configurationManager.getProperty<int>(
			"font_colour_g_int");
	int fontColourB = configurationManager.getProperty<int>(
			"font_colour_b_int");
	int fontColourA = configurationManager.getProperty<int>(
			"font_colour_a_int");
	int fontOutlineColourR = configurationManager.getProperty<int>(
			"font_outline_colour_r_int");
	int fontOutlineColourG = configurationManager.getProperty<int>(
			"font_outline_colour_g_int");
	int fontOutlineColourB = configurationManager.getProperty<int>(
			"font_outline_colour_b_int");
	int fontOutlineColourA = configurationManager.getProperty<int>(
			"font_outline_colour_a_int");

	float outlineWidth =
			configurationManager.getProperty<bool>("show_text_outline_bool") ?
					configurationManager.getProperty<float>(
							"font_outline_width_float") :
					0;

	int horizontalDPI = configurationManager.getProperty<int>(
			"font_dpi_horizontal_int");
	int verticalDPI = configurationManager.getProperty<int>(
			"font_dpi_vertical_int");

	int textPositionX = configurationManager.getProperty<int>("text_pos_x_int");
	int textPositionY = configurationManager.getProperty<int>("text_pos_y_int");
	float textSpacingY = configurationManager.getProperty<float>(
			"text_spacing_y_float");
	float textSpacingX = configurationManager.getProperty<float>(
			"text_spacing_x_float");

	fpsFormat = configurationManager.getProperty<boost::format>("fps_format");

	frameLoggingMessage = configurationManager.getProperty<std::string>(
			"frame_logging_message_string");

	renderer = new FontRenderer(fontName, fontSize, horizontalDPI, verticalDPI,
			outlineWidth);
	renderer->setFontColour(
			ColourRGBA(fontColourR, fontColourG, fontColourB, fontColourA));
	renderer->setFontOutlineColour(
			ColourRGBA(fontOutlineColourR, fontOutlineColourG,
					fontOutlineColourB, fontOutlineColourA));

	renderer->setTextPositionX(textPositionX);
	renderer->setTextPositionY(textPositionY);
	renderer->setTextSpacingX(textSpacingX);
	renderer->setTextSpacingY(textSpacingY);

	currentFrameCount = 0;
	framesPerSecond = 0;
//Set the time for the first time.
	previousTime = getMonotonicTimeNanoseconds() / 1000000ULL;
}

void OSDInstance::update(long currentMilliseconds) {

	framesPerSecond = 1000.0 * currentFrameCount
			/ (currentMilliseconds - previousTime);
	previousTime = currentMilliseconds;
	previousTime = currentMilliseconds;
	currentFrameCount = 0;

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

void OSDInstance::renderText(unsigned int width, unsigned int height) {
	std::stringstream osd_text_reader;
	osd_text_reader << osdText;
	if (GLXOSD::instance()->isFrameLoggingEnabled()) {
		osd_text_reader << std::endl << frameLoggingMessage;
	}

	std::string str = osd_text_reader.str();
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);

	renderer->render(width, height, str);
}

void OSDInstance::render(unsigned int width, unsigned int height) {
	currentFrameCount++;
	long currentTimeMilliseconds = getMonotonicTimeNanoseconds() / 1000000ULL;
//Refresh the info every 500 milliseconds
	if (currentTimeMilliseconds - previousTime >= 500) {
		update(currentTimeMilliseconds);
	}

	// Memorise misc. settings
	GLint program;
	{
		glGetIntegerv(GL_CURRENT_PROGRAM, &program);
		glUseProgram(0);
	}

	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	GLint frontFace;
	{
		glGetIntegerv(GL_FRONT_FACE, &frontFace);
		glFrontFace(GL_CCW);
	}

	GLint blendSrc;
	GLint blendDst;
	{
		glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendSrc);
		glGetIntegerv(GL_BLEND_DST_ALPHA, &blendDst);
	}

	GLfloat blendColour[4];
	{
		glGetFloatv(GL_BLEND_COLOR, blendColour);
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendColor(0, 0, 0, 0);

	GLboolean colourMask[4];
	{
		glGetBooleanv(GL_COLOR_WRITEMASK, colourMask);
		glColorMask(1, 1, 1, 1);
	}

	GLint depthFunc;
	{
		glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);
		glDepthFunc(GL_LESS);
	}

	//Equivalent to glPushAttrib -> glEnable/Disable -> glPopAttrib
	GLAttribState attribState(glEnable, glDisable, glIsEnabled);
	GLAttribState clientAttribState(glEnableClientState, glDisableClientState,
			glIsEnabled);
	{
		attribState.set(GL_ALPHA_TEST, GL_FALSE);
		attribState.set(GL_AUTO_NORMAL, GL_FALSE);
		attribState.set(GL_CULL_FACE, GL_FALSE);
		attribState.set(GL_COLOR_LOGIC_OP, GL_FALSE);
		attribState.set(GL_COLOR_TABLE, GL_FALSE);
		attribState.set(GL_CONVOLUTION_1D, GL_FALSE);
		attribState.set(GL_CONVOLUTION_2D, GL_FALSE);
		attribState.set(GL_CULL_FACE, GL_FALSE);
		attribState.set(GL_DEPTH_TEST, GL_FALSE);
		attribState.set(GL_DITHER, GL_FALSE);
		attribState.set(GL_FOG, GL_FALSE);
		attribState.set(GL_HISTOGRAM, GL_FALSE);
		attribState.set(GL_INDEX_LOGIC_OP, GL_FALSE);
		attribState.set(GL_LIGHTING, GL_FALSE);
		attribState.set(GL_NORMALIZE, GL_FALSE);
		attribState.set(GL_MINMAX, GL_FALSE);
		attribState.set(GL_SEPARABLE_2D, GL_FALSE);
		attribState.set(GL_SCISSOR_TEST, GL_FALSE);
		attribState.set(GL_STENCIL_TEST, GL_FALSE);
		attribState.set(GL_SAMPLE_ALPHA_TO_COVERAGE, GL_FALSE);
		attribState.set(GL_COLOR_LOGIC_OP, GL_FALSE);
		attribState.set(GL_CULL_FACE, GL_FALSE);
		attribState.set(GL_DEPTH_TEST, GL_FALSE);
		attribState.set(GL_MULTISAMPLE, GL_FALSE);
		attribState.set(GL_POLYGON_OFFSET_POINT, GL_FALSE);
		attribState.set(GL_POLYGON_OFFSET_LINE, GL_FALSE);
		attribState.set(GL_POLYGON_OFFSET_FILL, GL_FALSE);
		attribState.set(GL_SAMPLE_COVERAGE, GL_FALSE);
		attribState.set(GL_SCISSOR_TEST, GL_FALSE);
		attribState.set(GL_STENCIL_TEST, GL_FALSE);
		attribState.set(GL_TEXTURE_GEN_Q, GL_FALSE);
		attribState.set(GL_TEXTURE_GEN_R, GL_FALSE);
		attribState.set(GL_TEXTURE_GEN_S, GL_FALSE);
		attribState.set(GL_TEXTURE_GEN_T, GL_FALSE);
		clientAttribState.set(GL_COLOR_ARRAY, GL_FALSE);
		clientAttribState.set(GL_EDGE_FLAG_ARRAY, GL_FALSE);
		clientAttribState.set(GL_INDEX_ARRAY, GL_FALSE);
		clientAttribState.set(GL_NORMAL_ARRAY, GL_FALSE);
		clientAttribState.set(GL_TEXTURE_COORD_ARRAY, GL_TRUE);
		clientAttribState.set(GL_VERTEX_ARRAY, GL_TRUE);
	}
	{
		attribState.set(GL_TEXTURE_CUBE_MAP, GL_FALSE);
		attribState.set(GL_VERTEX_PROGRAM_ARB, GL_FALSE);
		attribState.set(GL_FRAGMENT_PROGRAM_ARB, GL_FALSE);
		attribState.set(GL_BLEND, GL_TRUE);
		attribState.set(GL_TEXTURE_2D, GL_TRUE);
	}

	GLint pixelUnpackBufferBinding = 0, arrayBufferBinding = 0, activeTexture =
			0, textureBinding2D = 0, vertexArrayBinding = 0, textureMinFilter =
			0, textureMagFilter = 0, textureWrapS = 0, textureWrapT = 0;

	//Memorise buffer states
	glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &pixelUnpackBufferBinding);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &arrayBufferBinding);
	glGetIntegerv(GL_ACTIVE_TEXTURE, &activeTexture);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &textureBinding2D);
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vertexArrayBinding);

	//Memorise texture parameters
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			&textureMinFilter);
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			&textureMagFilter);
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &textureWrapS);
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, &textureWrapT);

	renderText(width, height);

	//Revert texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureMinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureMagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapT);

	//Revert buffer states
	glActiveTexture(activeTexture);
	glBindTexture(GL_TEXTURE_2D, textureBinding2D);
	glBindVertexArray(vertexArrayBinding);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, pixelUnpackBufferBinding);
	glBindBuffer(GL_ARRAY_BUFFER, arrayBufferBinding);

	//Revert misc settings
	glDepthFunc(depthFunc);
	glBlendFunc(blendSrc, blendDst);
	glBlendColor(blendColour[0], blendColour[1], blendColour[2],
			blendColour[3]);
	glColorMask(colourMask[0], colourMask[1], colourMask[2], colourMask[3]);
	glFrontFace(frontFace);
	glUseProgram(program);
}
OSDInstance::~OSDInstance() {
}
}
