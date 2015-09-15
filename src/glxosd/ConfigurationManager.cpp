/*
 * Copyright (C) 2013-2014 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "ConfigurationManager.hpp"
#include "Utils.hpp"
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
// workaround X.h clash with Boost-xpressive < 1.53 (https://svn.boost.org/trac/boost/ticket/8204)
#undef None
#include <boost/xpressive/xpressive.hpp>
#include <fstream>
#include <iostream>
#include <iterator>
#include <utility>
#include <cmath>
namespace glxosd {

void ConfigurationManager::addDefaultConfigurationValue(std::string key,
		boost::any value) {
	defaultConfiguration.insert(std::pair<std::string, boost::any>(key, value));
}

ConfigurationManager::ConfigurationManager() {
	std::pair<int, int> dpi = getDPI();

	addDefaultConfigurationValue("font_name", std::string("CPMono_v07 Bold"));
	addDefaultConfigurationValue("font_size_int", 16);
	addDefaultConfigurationValue("font_colour_r_int", 255);
	addDefaultConfigurationValue("font_colour_g_int", 0);
	addDefaultConfigurationValue("font_colour_b_int", 255);
	addDefaultConfigurationValue("font_colour_a_int", 255);
	addDefaultConfigurationValue("font_outline_colour_r_int", 0);
	addDefaultConfigurationValue("font_outline_colour_g_int", 0);
	addDefaultConfigurationValue("font_outline_colour_b_int", 0);
	addDefaultConfigurationValue("font_outline_colour_a_int", 255);
	addDefaultConfigurationValue("font_outline_width_float", 1.0f);
	addDefaultConfigurationValue("font_dpi_horizontal_int", dpi.first);
	addDefaultConfigurationValue("font_dpi_vertical_int", dpi.second);
	addDefaultConfigurationValue("text_pos_x_int", 4);
	addDefaultConfigurationValue("text_pos_y_int", 2);
	addDefaultConfigurationValue("text_spacing_x_float", 0.0f);
	addDefaultConfigurationValue("text_spacing_y_float", 0.0f);
	addDefaultConfigurationValue("fps_format", glxosdFormat("FPS: %1$.1f\n"));
	addDefaultConfigurationValue("temperature_format",
			glxosdFormat("%1$.0f C"));
	addDefaultConfigurationValue("frame_logging_toggle_keycombo",
			std::string("Shift+F9"));
	addDefaultConfigurationValue("frame_logging_duration_ms", (uint64_t) 0ULL);
	addDefaultConfigurationValue("frame_logging_message_string",
			std::string("Logging frame timings..."));
	addDefaultConfigurationValue("frame_logging_dump_in_progress_message_string",
			std::string("Frame log dump in progress. Quitting now will result in data loss."));
	addDefaultConfigurationValue("frame_log_keep_in_memory_bool", false);
	addDefaultConfigurationValue("osd_toggle_keycombo",
			std::string("Shift+F10"));
	addDefaultConfigurationValue("vsync_toggle_keycombo",
			std::string("Shift+F11"));
	addDefaultConfigurationValue("frame_log_directory_string",
			std::string("/tmp/"));
	addDefaultConfigurationValue("show_text_outline_bool", true); // Deprecated
	configuration = readConfigChain();
}

const std::map<std::string, boost::any> ConfigurationManager::readConfigChain() {
	std::map<std::string, boost::any> configuration;

	std::string globalPath = "/etc/glxosd.conf";
	std::string userPath;
	std::string customPath = getEnvironment("GLXOSD_CONFIG_PATH");

	// Select user config location based on XDG basedir spec
	if (!getEnvironment("XDG_CONFIG_HOME").empty()) {
		userPath = getEnvironment("XDG_CONFIG_HOME") + "/glxosd/glxosd.conf";
	} else {
		userPath = getEnvironment("HOME") + "/.config/glxosd/glxosd.conf";
	}

	// Read the global config first...
	std::cout << "[GLXOSD] Reading global configuration file at \""
			<< globalPath << "\"..." << std::endl;
	readConfig(globalPath, configuration);

	// Then the user config...
	std::cout << "[GLXOSD] Reading user's configuration file at \"" << userPath
			<< "\"..." << std::endl;
	readConfig(userPath, configuration);

	if (!customPath.empty()) {
		// And finally, the config that is specified in the GLXOSD_CONFIG_PATH environment variable
		std::cout << "[GLXOSD] Reading user's configuration file at \""
				<< userPath << "\"..." << std::endl;
		readConfig(customPath, configuration);
	}

	return configuration;
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

void ConfigurationManager::readConfig(std::string path,
		std::map<std::string, boost::any>& configuration) {
	std::ifstream configFile(path.c_str());

	if (!configFile) {
		std::cout << "[GLXOSD] There is no file at \"" << path
				<< "\". Skipping." << std::endl;
		return;
	}

	while (!configFile.eof()) {
		std::string line;
		std::getline(configFile, line);
		std::string::size_type keyBegin = line.find_first_not_of(" \f\t\v");
		if (line.empty() || (keyBegin == std::string::npos) || (line[keyBegin] == '#')) { //Empty or comment
			continue;
		}
		std::string::size_type assignOp = line.find('=', keyBegin);
		std::string::size_type keyEnd = line.find_last_not_of(" \f\n\r\t\v",
				assignOp - 1);
		std::string key = line.substr(keyBegin, keyEnd - keyBegin + 1);
		std::string::size_type valueBegin = line.find_first_not_of(
				" \f\n\r\t\v", assignOp + 1);
		
		std::string value;
		
		if(valueBegin != std::string::npos)
		{
			std::string::size_type valueEnd = line.find_last_not_of(" \f\n\r\t\v")
					+ 1;
			value = line.substr(valueBegin, valueEnd - valueBegin);
		} else {
			value = "";
		}

		if ((value.size() > 1) && (value[0] == '"')
				&& (value[value.size() - 1] == '"')) {
			value = value.substr(1, value.size() - 2);
		}

		if ((value.size() > 1) && (value[0] == '\\') && (value[1] == '"')) {
			value = value.substr(1, value.size() - 1);
		}
		std::cout << "[GLXOSD] Found key-value pair: (key: \"" << key << "\""
				<< ", value: \"" << value << "\")" << std::endl;
		value = unescape(value);

		if (stringEndsWith(key, "_format")) {
			configuration[key] = glxosdFormat(value);
		} else if (stringEndsWith(key, "_filter")) {
			configuration[key] = boost::xpressive::sregex::compile(value,
					boost::xpressive::regex_constants::icase);
		} else if (stringEndsWith(key, "_bool")) {
			configuration[key] = (value == "true");
		} else if (stringEndsWith(key, "_int")) {
			configuration[key] = boost::lexical_cast<int>(value);
		} else if (stringEndsWith(key, "_ms")) {
			configuration[key] = boost::lexical_cast<uint64_t>(value);
		} else if (stringEndsWith(key, "_double")) {
			configuration[key] = boost::lexical_cast<double>(value);
		} else if (stringEndsWith(key, "_float")) {
			configuration[key] = boost::lexical_cast<float>(value);
		} else if (stringEndsWith(key, "_key")) {
			configuration[key] = boost::lexical_cast<std::string>(value);
		} else {
			configuration[key] = value;
		}
	}
	std::cout << "[GLXOSD] The configuration was read successfully."
			<< std::endl;
}

boost::any ConfigurationManager::__getProperty(const std::string key) const {
	if (configuration.find(key) == configuration.end()) {
		if (defaultConfiguration.find(key) != defaultConfiguration.end()) {
			return defaultConfiguration.at(key);
		}
		throw std::runtime_error("Missing GLXOSD property " + key);
	}

	return configuration.at(key);
}

}
