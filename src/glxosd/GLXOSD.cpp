#define GLX_GLXEXT_PROTOTYPES

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
#include "glinject.hpp"
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
bool frameLogDumpInProgress = false;
bool osdVisible = true;
bool toggleVsync = false;
KeyCombo frameLoggingToggleKey;
KeyCombo osdToggleKey;
KeyCombo vsyncToggleKey;

int frameLogId = 0;

uint64_t frameLogMonotonicTimeOffset = std::numeric_limits<uint64_t>::max();
std::string frameLogFilename = "";
std::string frameLogDirectory = "";
uint64_t frameLoggingDuration = 0;
std::ofstream frameLogStream;
std::vector<std::pair<GLXDrawable, uint64_t> > frameLog;
bool keepFrameLogInMemory = false;

bool frameLogToggledThisFrame = false;
bool osdToggledThisFrame = false;
bool vsyncToggledThisFrame = false;

/*
 * RAII is much better than try/finally, especially in this case! /s
 */
class GlinjectGLLockRAIIHelper {
public:
	GlinjectGLLockRAIIHelper() {
		glinject_lock_gl();
	}
	~GlinjectGLLockRAIIHelper() {
		glinject_unlock_gl();
	}
};

GLXOSD* GLXOSD::instance() {
	if (glxosdInstance == nullptr) {
		glxosdInstance = new GLXOSD();
	}
	return glxosdInstance;
}

GLXOSD::GLXOSD() {
	keyCombosInitialised = false;

	frameLogMutex = PTHREAD_MUTEX_INITIALIZER;

	configurationManager = new ConfigurationManager();

	frameLoggingDuration = getConfigurationManager().getProperty<uint64_t>(
			"frame_logging_duration_ms");
	frameLogDirectory = getConfigurationManager().getProperty<std::string>(
			"frame_log_directory_string");
	keepFrameLogInMemory = getConfigurationManager().getProperty<bool>(
			"frame_log_keep_in_memory_bool");

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

	if (toggleVsync)
	{
		unsigned int swapInterval;
		glXQueryDrawable(display, drawable, GLX_SWAP_INTERVAL_EXT, &swapInterval);
		swapInterval = (swapInterval > 0) ? 0 : 1;

		glXSwapIntervalEXT(display, drawable, swapInterval);
		toggleVsync = false;
	}

	if (osdVisible && display && drawable) {
		frameLoggingToggleKey = stringToKeyCombo(
			getConfigurationManager().getProperty<std::string>(
					"frame_logging_toggle_keycombo"), display);
		osdToggleKey = stringToKeyCombo(
			getConfigurationManager().getProperty<std::string>(
					"osd_toggle_keycombo"), display);
		
		keyCombosInitialised = true;

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
		GlinjectGLLockRAIIHelper raiiHelper;
		unsigned int windowWidth = 0, windowHeight = 0;
		glXQueryDrawable(display, drawable, GLX_WIDTH, &windowWidth);
		glXQueryDrawable(display, drawable, GLX_HEIGHT, &windowHeight);

		if (windowWidth < 1 || windowHeight < 1) {
			return;
		}

		GLint viewport[4];
		rgl(GetIntegerv)(GL_VIEWPORT, viewport);

		rgl(Viewport)(0, 0, windowWidth, windowHeight);

		instance->render(windowWidth, windowHeight);

		rgl(Viewport)(viewport[0], viewport[1], viewport[2], viewport[3]);
	}

	if (isFrameLoggingEnabled()) {
		frameLogTick(drawable);
	}
}
void GLXOSD::frameLogTick(GLXDrawable drawable) {
	if (frameLoggingDuration > 0
			&& frameLogMonotonicTimeOffset + frameLoggingDuration * 1000000
					< getMonotonicTimeNanoseconds()) {
		stopFrameLogging();
		return;
	}
	Lock lock(&frameLogMutex);
	if(keepFrameLogInMemory) {
		frameLog.push_back(
			std::pair<GLXDrawable, uint64_t>(
				drawable, 
				(getMonotonicTimeNanoseconds() - frameLogMonotonicTimeOffset)
			)
		);
	} else {
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
	Lock lock(&frameLogMutex);
	frameLoggingEnabled = true;
	createDirectory(frameLogDirectory);
	std::stringstream nameStream;
	nameStream << frameLogDirectory << "/glxosd_" << getpid() << "_"
			<< std::time(0) << "_" << frameLogId++ << ".log";
	frameLogFilename = nameStream.str();
	if(!keepFrameLogInMemory) {
		frameLogStream.open(frameLogFilename, std::ofstream::out);
	}
	frameLogMonotonicTimeOffset = getMonotonicTimeNanoseconds();
}
void GLXOSD::stopFrameLogging() {
	Lock lock(&frameLogMutex);
	frameLoggingEnabled = false;
	if(keepFrameLogInMemory) {
		frameLogDumpInProgress = true;
		frameLogStream.open(frameLogFilename, std::ofstream::out);
		for(auto frameLogEntry : frameLog){
			frameLogStream <<
				frameLogEntry.first << "," <<
				frameLogEntry.second << "\n";
		}
		frameLogStream.flush();
		frameLogDumpInProgress = false;
		frameLog.clear();
	}
	frameLogStream.close();
	frameLogStream.clear();
}
void GLXOSD::osdHandleKeyPress(XKeyEvent* event) {
	if (!frameLogToggledThisFrame
			&& keyCombosInitialised
			&& keyComboMatches(frameLoggingToggleKey, event)) {
		frameLogToggledThisFrame = true;
		if (frameLoggingEnabled)
			stopFrameLogging();
		else
			startFrameLogging();
	}
	if (!osdToggledThisFrame && keyCombosInitialised && keyComboMatches(osdToggleKey, event)) {
		osdToggledThisFrame = true;
		osdVisible = !osdVisible;
	}
	if (!vsyncToggledThisFrame && keyComboMatches(vsyncToggleKey, event)) {
		vsyncToggledThisFrame = false;
		toggleVsync = true;
	}
}

Bool GLXOSD::osdEventFilter(Display* display, XEvent* event, XPointer pointer) {
	if (event->type != KeyPress)
		return false;
	XKeyEvent * keyEvent = &event->xkey;
	return keyCombosInitialised && ( keyComboMatches(osdToggleKey, keyEvent)
			|| keyComboMatches(frameLoggingToggleKey, keyEvent) );
}

bool GLXOSD::isFrameLoggingEnabled() {
	return frameLoggingEnabled;
}

bool GLXOSD::isFrameLoggingDumpInProgress() {
	return frameLogDumpInProgress;
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

