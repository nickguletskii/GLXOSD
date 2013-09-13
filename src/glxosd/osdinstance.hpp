/*
 * Copyright (C) 2013 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef OSDINSTANCE_HPP_
#define OSDINSTANCE_HPP_
#ifdef FOUND_SENSORS
#include "standard_sensors.hpp"
#endif
#ifdef FOUND_NVCTRL
#include "nvidia_sensors.hpp"
#endif
#include <GL/gl.h>
#include <FTGL/ftgl.h>
#include <boost/format.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <string>
struct glxosd_config_type {
	std::string font_name;
	int font_size;
	int font_colour_r;
	int font_colour_g;
	int font_colour_b;
	int text_pos_x;
	int text_pos_y;
	int text_spacing_x;
	int text_spacing_y;
	bool show_text_outline;
	boost::format fps_format;
	boost::format chip_format;
	boost::format chip_feature_format;
	boost::format nvidia_gpu_format;
	boost::format temperature_format;
	boost::xpressive::sregex chip_feature_filter;
};
class osd_instance {
private:
	glxosd_config_type configuration;
	int current_frame_count; // The number of frames from the last FPS calculation
	long previous_time; //The time of the previous FPS calculation
	double frames_per_second;FTGLExtrdFont* font;
#ifdef FOUND_SENSORS
	std::string standard_sensors_string;
	standard_sensors* standard_sensors_access;
#endif
#ifdef FOUND_NVCTRL
	std::string nvidia_sensors_string;
	nvidia_sensors* nvidia_sensors_access;
#endif
public:
	osd_instance();
	void render(unsigned int width, unsigned int height);
	virtual ~osd_instance();
};

#endif /* OSDINSTANCE_HPP_ */
