/*
 * Copyright (C) 2013 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "standard_sensors.hpp"
#include <sensors/sensors.h>
#include <sstream>
#include <boost/lexical_cast.hpp>
standard_sensors::standard_sensors() {
	sensors_init(NULL);
}
std::string standard_sensors::get_sensors_info(boost::format chip_format,
		boost::format chip_feature_format, boost::format temperature_format,
		boost::xpressive::sregex feature_filter) {
	sensors_chip_name const * chip;
	int chip_number = 0;
	std::stringstream string_builder;
	while ((chip = sensors_get_detected_chips(0, &chip_number)) != 0) {
		char chip_name[256];
		sensors_snprintf_chip_name(chip_name, 256, chip);
		string_builder
				<< boost::str(
						boost::format(chip_format) % std::string(chip_name));
		sensors_feature const *feature;
		int feature_number = 0;
		while ((feature = sensors_get_features(chip, &feature_number)) != 0) {
			const sensors_subfeature *subfeature = sensors_get_subfeature(chip,
					feature, SENSORS_SUBFEATURE_TEMP_INPUT);
			std::string feature_name = sensors_get_label(chip, feature);
			if (boost::xpressive::regex_match(feature_name.begin(), feature_name.end(),
					feature_filter))
				continue;
			std::string temperature;
			if (subfeature) {
				double value;
				sensors_get_value(chip, subfeature->number, &value);
				if (value < 0)
					temperature = "unknown temperature";
				else
					temperature = boost::str(
							boost::format(temperature_format) % value);
			}
			string_builder
					<< boost::format(chip_feature_format) % feature_name
							% temperature;

		}
	}
	return string_builder.str();
}
standard_sensors::~standard_sensors() {
	sensors_cleanup();
}

