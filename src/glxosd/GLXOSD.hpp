/*
 * Copyright (C) 2013-2014 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef GLXOSD_HPP_
#define GLXOSD_HPP_
#include "OSDInstance.hpp"
#include "Wrapper.hpp"
#include "lock.hpp"
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <map>
#include <string>
#include <vector>

#define GLXOSD_PROPERTY GLXOSD::instance()->getConfigurationManager()->getProperty
namespace glxosd {
void osdHandleBufferSwap(Display* display, GLXDrawable drawable);
void osdHandleContextDestruction(Display* display, GLXContext context);
void osdHandleKeyPress(XKeyEvent* event);
Bool osdEventFilter(Display* display, XEvent* event, XPointer pointer);
class OSDInstance;
class ConfigurationManager;
class GLXOSD;

typedef Wrapper<void, glxosd::GLXOSD*> PluginConstructor;
typedef Wrapper<std::string*, glxosd::GLXOSD*> PluginDataProvider;
typedef Wrapper<void, glxosd::GLXOSD*> PluginDestructor;

class GLXOSD {
private:
	pthread_mutex_t frameLogMutex;
	pthread_mutex_t osdRenderingMutex;
	ConfigurationManager *configurationManager;
	std::map<GLXContext, glxosd::OSDInstance*>* drawableHandlers;
	std::vector<PluginConstructor>* pluginConstructors;
	std::vector<PluginDataProvider>* pluginDataProviders;
	std::vector<PluginDestructor>* pluginDestructors;
	std::vector<void*>* pluginSharedObjectHandles;
	void loadAllPlugins();
	void loadPlugin(std::string name);
	void startFrameLogging();
	void stopFrameLogging();
	void frameLogTick(GLXDrawable drawable);
	static GLXOSD *glxosdInstance;
	bool keyCombosInitialised;
public:
	static GLXOSD* instance();
	static void cleanup();
	GLXOSD();
	ConfigurationManager &getConfigurationManager();
	void osdHandleBufferSwap(Display* display, GLXDrawable drawable);
	void osdHandleContextDestruction(Display* display, GLXContext context);
	void osdHandleKeyPress(XKeyEvent* event);
	Bool osdEventFilter(Display* display, XEvent* event, XPointer pointer);
	bool isFrameLoggingEnabled();
	bool isFrameLoggingDumpInProgress();
	std::vector<PluginDataProvider>* getPluginDataProviders();
	virtual ~GLXOSD();
};
}
#endif
