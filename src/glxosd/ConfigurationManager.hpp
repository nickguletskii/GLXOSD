/*
 * Copyright (C) 2013-2014 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef CONFIGURATIONMANAGER_HPP_
#define CONFIGURATIONMANAGER_HPP_

#include <boost/any.hpp>
#include <map>
#include <string>
#include <vector>
#include <stdexcept>
namespace glxosd {
class ConfigurationManager {
private:
	std::map<std::string, boost::any> *defaultConfigurationCache;
	std::map<std::string, boost::any> *configurationCache;
	std::vector<std::pair<std::string, boost::any> > defaultConfiguration;
	std::map<std::string, boost::any> readConfigChain();
	void readConfig(std::string path,
			std::map<std::string, boost::any>& configuration);
	boost::any __getProperty(std::string key);
public:
	ConfigurationManager();
	void addDefaultConfigurationValue(std::string key, boost::any value);
	std::map<std::string, boost::any>* getConfiguration();
	std::map<std::string, boost::any>* getDefaultConfiguration();
	template<typename T>
	T getProperty(std::string key) {
		boost::any obj = __getProperty(key);
		if (obj.type() != typeid(T)) {
			throw std::runtime_error(
					"GLXOSD property " + key
							+ " has an incorrect type! Expected "
							+ typeid(T).name() + ", got " + obj.type().name());
		}
		T val = boost::any_cast<T>(obj);
		return val;
	}
};
}
#endif
