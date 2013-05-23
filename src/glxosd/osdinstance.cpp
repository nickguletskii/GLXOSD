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

int string_to_int(std::string str) {
	std::stringstream ss(str);
	int res;
	ss >> res;
	return res;
}
void read_config_into_struct(std::string path,
		glxosd_config_type* configuration) {
	std::ifstream config_file(path.c_str());
	if (!config_file) {
		std::cout << "[GLXOSD]: There is no file at \"" << path
				<< "\". Skipping." << std::endl;
		return;
	}
	while (!config_file.eof()) {
		std::string line;
		std::getline(config_file, line);
		std::string::size_type key_begin = line.find_first_not_of(" \f\t\v");
		if (key_begin == std::string::npos || line[key_begin] == '#') //Empty line or comment
			continue;
		std::string::size_type assign_op = line.find('=', key_begin);
		std::string::size_type key_end = line.find_last_not_of(" \f\n\r\t\v",
				assign_op - 1);
		std::string key = line.substr(key_begin, key_end - key_begin + 1);
		std::string::size_type value_begin = line.find_first_not_of(
				" \f\n\r\t\v", assign_op + 1);
		std::string::size_type value_end = line.find_last_not_of(" \f\n\r\t\v")
				+ 1;
		std::string value = line.substr(value_begin, value_end - value_begin);
		std::cout << "[GLXOSD]: Found key-value pair: (key: \"" << key << "\""
				<< ", value: \"" << value << "\")" << std::endl;
		if (key == "font_name")
			configuration->font_name = value;
		else if (key == "font_size")
			configuration->font_size = string_to_int(value);
		else if (key == "font_colour_r")
			configuration->font_colour_r = string_to_int(value);
		else if (key == "font_colour_g")
			configuration->font_colour_g = string_to_int(value);
		else if (key == "font_colour_b")
			configuration->font_colour_b = string_to_int(value);
		else if (key == "text_format")
			configuration->text_format = value;
		else if (key == "show_text_outline")
			configuration->show_text_outline = (value == "true");
		else
			std::cout << "[GLXOSD]: Unknown key: \"" << key << "\""
					<< std::endl;
	}
	std::cout << "[GLXOSD]: The configuration was read successfully."
			<< std::endl;
}
glxosd_config_type load_config() {
	glxosd_config_type configuration = { "SquareFont", 16, 255, 0, 255, true,
			"FPS: %.1f" };
	std::string global_path = "/etc/glxosd.conf";
	std::string user_path = std::string(getenv("HOME")) + "/.glxosd/glxosd.conf";
	std::cout << "[GLXOSD]: Reading global configuration file at \""
			<< global_path << "\"..." << std::endl;
	read_config_into_struct(global_path, &configuration);
	std::cout << "[GLXOSD]: Reading user's configuration file at \""
			<< user_path << "\"..." << std::endl;
	read_config_into_struct(user_path, &configuration);
	return configuration;
}
const std::string find_font(const char* name) {
	std::string font_file;
	FcConfig* config = FcInitLoadConfigAndFonts();
	FcPattern* pattern = FcNameParse((const FcChar8*) (name));
	FcConfigSubstitute(config, pattern, FcMatchPattern);
	FcDefaultSubstitute(pattern);
	FcPattern* font = FcFontMatch(config, pattern, NULL);
	if (font) {
		FcChar8* file = NULL;
		if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch)
			font_file = std::string((char*) file);
		FcPatternDestroy(font);
	}
	FcPatternDestroy(pattern);
	return font_file;
}
osd_instance::osd_instance() {
	configuration = load_config();
	//Font initialisation, obviously...
	std::string font_path = find_font(configuration.font_name.c_str());
	std::cout << "[GLXOSD]: Loading font \"" << font_path << "\" with size "
			<< configuration.font_size << "..." << std::endl;
	font = new FTGLExtrdFont(font_path.c_str());
	font->FaceSize(configuration.font_size);
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
	char osd_text_c[256];
	sprintf(osd_text_c, configuration.text_format.c_str(), frames_per_second);
	std::string osd_text(osd_text_c);
	//Trick from http://gamedev.stackexchange.com/a/46512
	glColor3ub(configuration.font_colour_r, configuration.font_colour_g,
			configuration.font_colour_b);
	font->Render(osd_text.c_str(), osd_text.size(), FTPoint(4, height - 20, 0),
			FTPoint(), FTGL::RENDER_FRONT);
	if (configuration.show_text_outline) {
		glColor3ub(0, 0, 0);
		font->Render(osd_text.c_str(), osd_text.size(),
				FTPoint(4, height - 20, 0), FTPoint(), FTGL::RENDER_SIDE);
	}
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glClear(GL_DEPTH_BUFFER_BIT);
	glPopAttrib();
	glPopMatrix();
}
