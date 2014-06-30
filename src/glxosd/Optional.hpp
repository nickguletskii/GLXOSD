/*
 * Copyright (C) 2013-2014 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef OPTIONAL_HPP_
#define OPTIONAL_HPP_

namespace glxosd {

template<typename T>
class Optional {
public:
	Optional() :
			value(nullptr) {
	}
	Optional(const T *arg) {
		value = arg;
	}
	const T& operator or(const T &x) {
		if (value == nullptr)
			return x;
		return *value;
	}
	const T* get() {
		return value;
	}
	bool isNull() {
		return value == nullptr;
	}
	bool isPresent() {
		return value != nullptr;
	}
	~Optional() {
		delete value;
	}
private:
	const T * value;
};

} /* namespace glxosd */

#endif /* OPTIONAL_HPP_ */
