/*
 * Copyright (C) 2013 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "ConfigurationManager.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <fstream>
namespace glxosd {
std::map<std::string, boost::any> defaultConfiguration;

void addDefaultConfigurationValue(std::string key, boost::any value) {
	defaultConfiguration[key] = value;
}

void initDefaultConfiguration() {
	defaultConfiguration["font_name"] = std::string("SquareFont");
	defaultConfiguration["font_size_int"] = 16;
	defaultConfiguration["font_colour_r_int"] = 255;
	defaultConfiguration["font_colour_g_int"] = 0;
	defaultConfiguration["font_colour_b_int"] = 255;
	defaultConfiguration["text_pos_x_int"] = 4;
	defaultConfiguration["text_pos_y_int"] = 2;
	defaultConfiguration["text_spacing_x_int"] = 2;
	defaultConfiguration["text_spacing_y_int"] = 2;
	defaultConfiguration["show_text_outline_bool"] = true;
	defaultConfiguration["fps_format"] = boost::format("FPS: %1$.1f\n");
	defaultConfiguration["temperature_format"] = boost::format("%1$i C");
}
static std::string unescape(std::string s) {
	std::string result;
	for (std::string::const_iterator i = s.begin(); i != s.end(); i++) {
		char c = *i;
		if ((c == '\\') && (i != s.end())) {
			i++;
			if (i == s.end()) {
				return result;
			}
			switch (*i) {
			case '\\':
				c = '\\';
				break;
			case 'n':
				c = '\n';
				break;
			default:
				continue;
			}
		}
		result += c;
	}
	return result;
}

static bool stringEndsWith(std::string const& fullString,
		std::string const& ending) {
	if (fullString.length() >= ending.length()) {
		return fullString.compare(fullString.length() - ending.length(),
				ending.length(), ending) == 0;
	} else {
		return false;
	}
}

extern void readConfig(std::string path,
		std::map<std::string, boost::any>& configuration) {
	std::ifstream configFile(path.c_str());

	if (!configFile) {
		std::cout << "[GLXOSD]: There is no file at \"" << path
				<< "\". Skipping." << std::endl;
		return;
	}

	while (!configFile.eof()) {
		std::string line;
		std::getline(configFile, line);
		std::string::size_type keyBegin = line.find_first_not_of(" \f\t\v");
		if ((keyBegin == std::string::npos) || (line[keyBegin] == '#')) { //Empty or comment
			continue;
		}
		std::string::size_type assignOp = line.find('=', keyBegin);
		std::string::size_type keyEnd = line.find_last_not_of(" \f\n\r\t\v",
				assignOp - 1);
		std::string key = line.substr(keyBegin, keyEnd - keyBegin + 1);
		std::string::size_type valueBegin = line.find_first_not_of(
				" \f\n\r\t\v", assignOp + 1);
		std::string::size_type valueEnd = line.find_last_not_of(" \f\n\r\t\v")
				+ 1;
		std::string value = line.substr(valueBegin, valueEnd - valueBegin);

		if ((value.size() > 1) && (value[0] == '"')
				&& (value[value.size() - 1] == '"')) {
			value = value.substr(1, value.size() - 2);
		}

		if ((value.size() > 1) && (value[0] == '\\') && (value[1] == '"')) {
			value = value.substr(1, value.size() - 1);
		}
		std::cout << "[GLXOSD]: Found key-value pair: (key: \"" << key << "\""
				<< ", value: \"" << value << "\")" << std::endl;
		value = unescape(value);

		if (stringEndsWith(key, "_format")) {
			configuration[key] = boost::format(value);
		} else if (stringEndsWith(key, "_filter")) {
			configuration[key] = boost::xpressive::sregex::compile(value,
					boost::xpressive::regex_constants::icase);
		} else if (stringEndsWith(key, "_bool")) {
			configuration[key] = (value == "true");
		} else if (stringEndsWith(key, "_int")) {
			configuration[key] = boost::lexical_cast<int>(value);
		} else if (stringEndsWith(key, "_double")) {
			configuration[key] = boost::lexical_cast<double>(value);
		} else {
			configuration[key] = value;
		}
	}
	std::cout << "[GLXOSD]: The configuration was read successfully."
			<< std::endl;
}

extern std::map<std::string, boost::any> readConfigChain() {
	std::map<std::string, boost::any> configuration(defaultConfiguration);

	std::string globalPath = "/etc/glxosd.conf";
	std::string userPath = std::string(getenv("HOME")) + "/.glxosd/glxosd.conf";

	// Read the global config first...
	std::cout << "[GLXOSD]: Reading global configuration file at \""
			<< globalPath << "\"..." << std::endl;
	readConfig(globalPath, configuration);

	// Then the user config...
	std::cout << "[GLXOSD]: Reading user's configuration file at \"" << userPath
			<< "\"..." << std::endl;
	readConfig(userPath, configuration);

	return configuration;
}
}
