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
#include "Utils.hpp"
#include "OSDInstance.hpp"
#include "ConfigurationManager.hpp"
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>

namespace glxosd {

GLXOSD* GLXOSD::glxosdInstance = nullptr;
bool frameLoggingEnabled = false;
bool osdVisible = true;
KeyCombo frameLoggingToggleKey;
KeyCombo osdToggleKey;

int frameLogId = 0;

uint64_t frameLogMonotonicTimeOffset = std::numeric_limits<uint64_t>::max();
std::string frameLogFilename = "";
std::string frameLogDirectory = "";
std::ofstream frameLogStream;

bool frameLogToggledThisFrame = false;
bool osdToggledThisFrame = false;

GLXOSD* GLXOSD::instance() {
	if (glxosdInstance == nullptr) {
		glxosdInstance = new GLXOSD();
	}
	return glxosdInstance;
}

GLXOSD::GLXOSD() {
	configurationManager = new ConfigurationManager();

	frameLoggingToggleKey = stringToKeyCombo(
			getConfigurationManager().getProperty<std::string>(
					"frame_logging_toggle_keycombo"));
	osdToggleKey = stringToKeyCombo(
			getConfigurationManager().getProperty<std::string>(
					"osd_toggle_keycombo"));
	frameLogDirectory = getConfigurationManager().getProperty<std::string>(
			"frame_log_directory_string");

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
	osdToggledThisFrame = false;
	frameLogToggledThisFrame = false;

	unsigned int width = 1;
	unsigned int height = 1;

	if (osdVisible && display && drawable) {

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

	if (isFrameLoggingEnabled()) {
		frameLogStream << drawable << ","
				<< (getMonotonicTimeNanoseconds() - frameLogMonotonicTimeOffset)
				<< std::endl;
	}
}
void GLXOSD::osdHandleContextDestruction(Display* display, GLXContext context) {
	auto it = drawableHandlers->find(context);
	if (it != drawableHandlers->end()) {
		delete (*it).second;
		drawableHandlers->erase(it);
	}
}
void createDirectory(std::string path) {
	for (size_t i = 1; i < path.length(); i++) {
		if (path[i] == '/') {
			int status = mkdir(path.substr(0, i).c_str(),
			S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			if (status == 0)
				continue;

			int errsv = errno;
			switch (errsv) {
			case EACCES:
				throw std::runtime_error(
						"Couldn't create directory " + path
								+ ": access denied.");
			case EEXIST:
				goto next;
			case ELOOP:
				throw std::runtime_error(
						"Couldn't create directory " + path
								+ ": loop in the tree.");
			case EMLINK:
				throw std::runtime_error(
						"Couldn't create directory " + path
								+ ": the link count of the parent directory would exceed {LINK_MAX}.");
			case ENAMETOOLONG:
				throw std::runtime_error(
						"Couldn't create directory " + path
								+ ": the length of the path argument exceeds {PATH_MAX} or a pathname component is longer than {NAME_MAX}.");
			case ENOSPC:
				throw std::runtime_error(
						"Couldn't create directory " + path
								+ ": the file system does not contain enough space to hold the contents of the new directory or to extend the parent directory of the new directory.");
			case ENOTDIR:
				throw std::runtime_error(
						"Couldn't create directory " + path
								+ ": a component of the path is not a directory.");
			case EROFS:
				throw std::runtime_error(
						"Couldn't create directory " + path
								+ ": read only filesystem.");
			}
		}
		next: ;
	}

}
void GLXOSD::startFrameLogging() {
	createDirectory(frameLogDirectory);
	std::stringstream nameStream;
	nameStream << frameLogDirectory << "/" << getpid() << "_" << std::time(0)
			<< "_" << frameLogId++ << ".log";
	frameLogFilename = nameStream.str();
	frameLogStream.open(frameLogFilename, std::ofstream::out);
	frameLogMonotonicTimeOffset = getMonotonicTimeNanoseconds();
}
void GLXOSD::stopFrameLogging() {
	frameLogStream.close();
	frameLogStream.clear();
}
void GLXOSD::osdHandleKeyPress(XKeyEvent* event) {
	if (!frameLogToggledThisFrame
			&& keyComboMatches(frameLoggingToggleKey, event)) {
		frameLogToggledThisFrame = true;
		frameLoggingEnabled = !frameLoggingEnabled;
		if (frameLoggingEnabled)
			startFrameLogging();
		else
			stopFrameLogging();
	}
	if (!osdToggledThisFrame && keyComboMatches(osdToggleKey, event)) {
		osdToggledThisFrame = true;
		osdVisible = !osdVisible;
	}
}

Bool GLXOSD::osdEventFilter(Display* display, XEvent* event, XPointer pointer) {
	if (event->type != KeyPress)
		return false;
	XKeyEvent * keyEvent = &event->xkey;
	return keyComboMatches(osdToggleKey, keyEvent)
			|| keyComboMatches(frameLoggingToggleKey, keyEvent);
}

bool GLXOSD::isFrameLoggingEnabled() {
	return frameLoggingEnabled;
}

ConfigurationManager & GLXOSD::getConfigurationManager() {
	return *configurationManager;
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

std::vector<PluginDataProvider> *GLXOSD::getPluginDataProviders() {
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
void osdHandleKeyPress(XKeyEvent* event) {
	GLXOSD::instance()->osdHandleKeyPress(event);
}
Bool osdEventFilter(Display* display, XEvent* event, XPointer pointer) {
	return GLXOSD::instance()->osdEventFilter(display, event, pointer);
}

}

