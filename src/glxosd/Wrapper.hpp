/*
 * Copyright (C) 2013-2014 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef WRAPPER_HPP_
#define WRAPPER_HPP_
#include <dlfcn.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <functional>
namespace glxosd {

template<typename Ret, typename ... Args>
class Wrapper {
public:
	Wrapper(void* sharedObjectHandle, std::string name) :
			name(name) {
		dlerror();
		*reinterpret_cast<void**>(&fptr) = dlsym(sharedObjectHandle,
				name.c_str());
		if (fptr == nullptr) {
			std::string error = std::string(dlerror());
			// Can't use std::cerr and std::runtime_exception here because stderr hangs for some reason :(
			std::cout
					<< "[GLXOSD] Critical failure: Could not load plugin's function "
					<< name << ": " << error << std::endl;
			exit(EXIT_FAILURE);
		}
		fct = std::function<Ret(Args...)>(fptr);
	}

	~Wrapper() {
	}
	Ret operator()(Args ... args) {
		return fct(std::forward<Args...>(args...));
	}
private:
	Ret (*fptr)(Args...);
	std::function<Ret(Args...)> fct;
	std::string name;

};

}

#endif
