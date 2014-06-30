/*
 * Copyright (C) 2013-2014 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "GLXOSD.hpp"
#include "LibsensorsSensorDataProvider.hpp"
#include "ConfigurationManager.hpp"
#include <boost/format.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <sensors/sensors.h>
#include <sstream>
#include <string>

boost::format chipFormat;
boost::format libsensorsChipFeatureFormat;
boost::xpressive::sregex libsensorsChipFeatureFilter;
boost::format temperatureFormat;
void glxosdPluginConstructor(glxosd::GLXOSD *glxosd) {
	glxosd::ConfigurationManager &configurationManager =
			glxosd->getConfigurationManager();
	configurationManager.addDefaultConfigurationValue("libsensors_chip_format",
			boost::format("%1%:\n"));
	configurationManager.addDefaultConfigurationValue(
			"libsensors_chip_feature_format", boost::format(" %1%: %2%\n"));
	configurationManager.addDefaultConfigurationValue(
			"libsensors_chip_feature_filter",
			boost::xpressive::sregex::compile("Core.*",
					boost::xpressive::regex_constants::icase));

	chipFormat = configurationManager.getProperty<boost::format>(
			"libsensors_chip_format");
	libsensorsChipFeatureFormat =
			configurationManager.getProperty<boost::format>(
					"libsensors_chip_feature_format");
	libsensorsChipFeatureFilter = configurationManager.getProperty<
			boost::xpressive::sregex>("libsensors_chip_feature_filter");
	temperatureFormat = configurationManager.getProperty<boost::format>(
			"temperature_format");

	sensors_init(NULL);
}

std::string* glxosdPluginDataProvider(glxosd::GLXOSD *glxosdInstance) {

	sensors_chip_name const * chip;
	int chipNumber = 0;

	std::stringstream stringBuilder;
	while ((chip = sensors_get_detected_chips(0, &chipNumber)) != 0) {

		char chipName[256];
		sensors_snprintf_chip_name(chipName, 256, chip);

		stringBuilder
				<< boost::str(
						boost::format(chipFormat) % std::string(chipName));

		sensors_feature const *feature;
		int feature_number = 0;

		while ((feature = sensors_get_features(chip, &feature_number)) != 0) {

			const sensors_subfeature *subfeature = sensors_get_subfeature(chip,
					feature, SENSORS_SUBFEATURE_TEMP_INPUT);
			std::string feature_name = sensors_get_label(chip, feature);

			if (boost::xpressive::regex_match(feature_name.begin(),
					feature_name.end(), libsensorsChipFeatureFilter))
				continue;

			std::string temperature;

			if (subfeature) {

				double value;
				sensors_get_value(chip, subfeature->number, &value);

				if (value < 0) {
					temperature = "unknown temperature";
				} else {
					temperature = boost::str(
							boost::format(temperatureFormat) % value);
				}
			}

			stringBuilder
					<< boost::format(libsensorsChipFeatureFormat) % feature_name
							% temperature;

		}
	}
	return new std::string(stringBuilder.str());
}
void glxosdPluginDestructor(glxosd::GLXOSD *glxosd) {
	sensors_cleanup();
}

