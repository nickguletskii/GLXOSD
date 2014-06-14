/*
 * Copyright (C) 2013-2014 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "Utils.hpp"
#include <ConfigurationManager.hpp>
#include <dlfcn.h>
#include <GLXOSD.hpp>
#include <OSDInstance.hpp>
#include <X11/Xlib.h>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <utility>
#include <vector>
#include <sstream>
namespace glxosd {
GLXOSD* GLXOSD::glxosdInstance = 0;
GLXOSD* GLXOSD::instance() {
	if (glxosdInstance == 0) {
		glxosdInstance = new GLXOSD();
	}
	return glxosdInstance;
}

GLXOSD::GLXOSD() {
	configurationManager = new ConfigurationManager();
	drawableHandlers = new std::map<GLXContext, glxosd::OSDInstance*>;
	pluginConstructors = new std::vector<PluginConstructor>();
	pluginDataProviders = new std::vector<PluginDataProvider>();
	pluginDestructors = new std::vector<PluginDestructor>();
	pluginSharedObjectHandles = new std::vector<void*>();
	loadAllPlugins();
	std::for_each(pluginConstructors->begin(), pluginConstructors->end(),
			[this](PluginConstructor pluginConstructor) {pluginConstructor(this);});
}

void GLXOSD::osdHandleBufferSwap(Display* display, GLXDrawable drawable) {
	unsigned int width = 1;
	unsigned int height = 1;

	if (display && drawable) {

		auto it = drawableHandlers->find(glXGetCurrentContext());

		OSDInstance* instance;

		if (it == drawableHandlers->end()) {
			instance = new OSDInstance();
			drawableHandlers->insert(
					std::pair<GLXContext, glxosd::OSDInstance*>(
							glXGetCurrentContext(), instance));
		} else {
			instance = (*it).second;
		}

		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		width = viewport[2];
		height = viewport[3];

		if (width < 1 || height < 1) {
			return;
		}

		instance->render(width, height);
	}
}
void GLXOSD::osdHandleContextDestruction(Display* display, GLXContext context) {
	auto it = drawableHandlers->find(context);
	if (it != drawableHandlers->end()) {
		delete (*it).second;
		drawableHandlers->erase(it);
	}
}
ConfigurationManager* GLXOSD::getConfigurationManager() {
	return configurationManager;
}

void GLXOSD::cleanup() {
	if (glxosdInstance == nullptr) {
		return;
	}
	delete glxosdInstance;
}

GLXOSD::~GLXOSD() {
	for_each(pluginDestructors->begin(), pluginDestructors->end(),
			[this](PluginDestructor destructor) {
				destructor(this);
			});
	delete configurationManager;
	for_each(pluginSharedObjectHandles->begin(),
			pluginSharedObjectHandles->end(), [](void* handle) {
				dlclose(handle);
			});
	delete pluginSharedObjectHandles;
	delete pluginDataProviders;
	delete drawableHandlers;
	glxosdInstance = nullptr;
}

/*
 * Plugin function getters
 */

std::vector<PluginDataProvider>* GLXOSD::getPluginDataProviders() {
	return pluginDataProviders;
}

/*
 * Plugin management
 */

void GLXOSD::loadPlugin(std::string name) {
	std::cout << "[GLXOSD] Loading " << name << std::endl;
	void *handle = dlopen(name.c_str(), RTLD_LAZY);
	if (!handle) {
		throw std::runtime_error(
				"[GLXOSD] Could not load plugin: " + std::string(dlerror()));
	}

	pluginSharedObjectHandles->push_back(handle);

	pluginConstructors->push_back(
			PluginConstructor(handle, "glxosdPluginConstructor"));
	pluginDataProviders->push_back(
			PluginDataProvider(handle, "glxosdPluginDataProvider"));
	pluginDestructors->push_back(
			PluginDestructor(handle, "glxosdPluginDestructor"));

	std::cout << "[GLXOSD] Loaded " << name << std::endl;
}
void GLXOSD::loadAllPlugins() {
	std::istringstream ss(getEnvironment("GLXOSD_PLUGINS"));
	std::string token;
	while (std::getline(ss, token, ':')) {
		if (token.empty()) {
			continue;
		}
		loadPlugin(token);
	}
}

/*
 * Wrappers
 */

void osdHandleBufferSwap(Display* display, GLXDrawable drawable) {
	GLXOSD::instance()->osdHandleBufferSwap(display, drawable);
}
void osdHandleContextDestruction(Display* display, GLXContext context) {
	GLXOSD::instance()->osdHandleContextDestruction(display, context);
}

}
