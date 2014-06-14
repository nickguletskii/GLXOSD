/*
 * Copyright (C) 2013-2014 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "NvidiaSensorDataProvider.hpp"
#include "ConfigurationManager.hpp"
#include "GLXOSD.hpp"
#include "OSDInstance.hpp"
#include <sstream>
#include <NVCtrl/NVCtrl.h>
#include <NVCtrl/NVCtrlLib.h>

int numberOfGpus;
Display *display;
std::string errorResult;
void glxosdPluginConstructor(glxosd::GLXOSD *glxosd) {
	glxosd->getConfigurationManager()->addDefaultConfigurationValue(
			"nvidia_gpu_format", boost::format("%1% (%2%): %3%\n"));
	int event, error;

	display = XOpenDisplay(NULL);

	if (!display || !XNVCTRLQueryExtension(display, &event, &error)
			|| !XNVCTRLQueryTargetCount(display, NV_CTRL_TARGET_TYPE_GPU,
					&numberOfGpus)) {
		errorResult = "Couldn't read the number of NVIDIA GPUs.";
	}
}

std::string* glxosdPluginDataProvider(glxosd::GLXOSD *glxosdInstance) {
	if (!errorResult.empty()) {
		return new std::string(errorResult);
	}
	glxosd::ConfigurationManager* configurationManager =
			glxosdInstance->getConfigurationManager();
	std::stringstream stringBuilder;

	for (int i = 0; i < numberOfGpus; i++) {
		char *name;
		int temperature;

		if ((XNVCTRLQueryTargetAttribute(display,
		NV_CTRL_TARGET_TYPE_GPU, i, 0,
		NV_CTRL_GPU_CORE_TEMPERATURE, &temperature) != True)
				|| (XNVCTRLQueryTargetStringAttribute(display,
				NV_CTRL_TARGET_TYPE_GPU, i, 0,
				NV_CTRL_STRING_PRODUCT_NAME, &name) != True)) {
			stringBuilder
					<< (boost::format(
							configurationManager->getProperty < boost::format
									> ("nvidia_gpu_format"))) % "unknown" % i
							% "failed to get the temperature!";
		} else {
			stringBuilder
					<< boost::format(

							configurationManager->getProperty < boost::format
									> ("nvidia_gpu_format")) % name % i
							% (boost::format(
									glxosdInstance->getConfigurationManager()->getProperty<
											boost::format>(
											"temperature_format")) % temperature);
		}
	}

	return new std::string(stringBuilder.str());
}

void glxosdPluginDestructor(glxosd::GLXOSD *glxosd) {
	if (display) {
		XCloseDisplay(display);
		display = nullptr;
	}
}

