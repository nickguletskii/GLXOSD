/*
 * Copyright (C) 2013 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "OSDInstance.hpp"
#include "OSD.hpp"
#include <fstream>
#include <map>
std::map<GLXContext, glxosd::OSDInstance*>* drawableHandlers; //Stores the instances of the OSD. Each GLX context has its own.
void startup() {
	drawableHandlers = new std::map<GLXContext, glxosd::OSDInstance*>;
	gl_frame_handler glHandler = { &osdHandleBufferSwap,
			&osdHandleContextDestruction };
	glinject_add_gl_frame_handler(glHandler);
}

void osdHandleBufferSwap(Display* display, GLXDrawable drawable) {
	unsigned int width = 1;
	unsigned int height = 1;
	if (display && drawable) {
		std::map<GLXContext, glxosd::OSDInstance*>::iterator it =
				drawableHandlers->find(glXGetCurrentContext());
		glxosd::OSDInstance* instance;
		if (it == drawableHandlers->end()) {
			instance = new glxosd::OSDInstance();
			drawableHandlers->insert(
					std::pair<GLXContext, glxosd::OSDInstance*>(
							glXGetCurrentContext(), instance));
		} else
			instance = (*it).second;
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		width = viewport[2];
		height = viewport[3];
		if (width < 1 || height < 1)
			return;
		instance->render(width, height);
	}
}
void osdHandleContextDestruction(Display* display, GLXContext context) {
	std::map<GLXContext, glxosd::OSDInstance*>::iterator it =
			drawableHandlers->find(context);
	if (it != drawableHandlers->end()) {
		delete (*it).second;
		drawableHandlers->erase(it);
	}
}

