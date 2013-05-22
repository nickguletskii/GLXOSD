/*
 * Copyright (C) 2013 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "osdinstance.hpp"
#include "osd.hpp"
#include <fstream>
#include <map>
std::map<GLXContext, osd_instance*>* drawable_handlers; //Stores the instances of the OSD. Each GLX context has its own.
void startup() {
	drawable_handlers = new std::map<GLXContext, osd_instance*>;
	gl_frame_handler gl_handler = { &osd_handle_buffer_swap,
			&osd_handle_context_destruction };
	add_gl_frame_handler(gl_handler);
}

void osd_handle_buffer_swap(Display* display, GLXDrawable drawable) {
	unsigned int width = 1;
	unsigned int height = 1;
	if (display && drawable) {
		std::map<GLXContext, osd_instance*>::iterator it =
				drawable_handlers->find(glXGetCurrentContext());
		osd_instance* instance;
		if (it == drawable_handlers->end()) {
			instance = new osd_instance();
			drawable_handlers->insert(
					std::pair<GLXContext, osd_instance*>(glXGetCurrentContext(),
							instance));
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
void osd_handle_context_destruction(Display* display, GLXContext context) {
	std::map<GLXContext, osd_instance*>::iterator it = drawable_handlers->find(
			context);
	if (it != drawable_handlers->end()) {
		delete (*it).second;
		drawable_handlers->erase(it);
	}
}
