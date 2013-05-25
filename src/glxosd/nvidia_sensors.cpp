/*
 * Copyright (C) 2013 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "nvidia_sensors.hpp"
#include <sstream>
#include <NVCtrl/NVCtrl.h>
#include <NVCtrl/NVCtrlLib.h>

nvidia_sensors::nvidia_sensors() {
	int event, error;
	display = XOpenDisplay(NULL);
	if (!display || !XNVCTRLQueryExtension(display, &event, &error)
			|| !XNVCTRLQueryTargetCount(display, NV_CTRL_TARGET_TYPE_GPU,
					&number_of_gpus))
		error_result = "Couldn't read the number of NVIDIA GPUs.";
}
std::string nvidia_sensors::get_sensors_info(boost::format format,
		boost::format temperature_format) {
	if (!error_result.empty())
		return error_result;
	std::stringstream string_builder;
	for (int i = 0; i < number_of_gpus; i++) {
		char* name;
		int temperature;
		if (XNVCTRLQueryTargetAttribute(display, NV_CTRL_TARGET_TYPE_GPU, i, 0,
				NV_CTRL_GPU_CORE_TEMPERATURE, &temperature) != True
				|| XNVCTRLQueryTargetStringAttribute(display,
						NV_CTRL_TARGET_TYPE_GPU, i, 0,
						NV_CTRL_STRING_PRODUCT_NAME, &name) != True)
			string_builder
					<< boost::format(format) % "unknown" % i
							% "failed to get the temperature!";
		else
			string_builder
					<< boost::format(format) % name % i
							% (boost::format(temperature_format) % temperature);
	}
	return string_builder.str();
}
nvidia_sensors::~nvidia_sensors() {
	if (display) {
		XCloseDisplay(display);
		display = NULL;
	}
}

