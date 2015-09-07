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
#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <X11/Xlib.h>
#include <X11/keysym.h>
namespace glxosd {
std::string getEnvironment(const std::string & var) {
	const char * val = getenv(var.c_str());
	return (val == nullptr) ? "" : val;
}

std::pair<int, int> getDPI() {
	Display * dpy = XOpenDisplay(nullptr);
	int screen = 0; //TODO: find a way to get the current screen
	int horizontalDPI = ((((double) XDisplayWidth(dpy, screen)) * 25.4)
			/ ((double) XDisplayWidthMM(dpy, screen)));
	int verticalDPI = ((((double) XDisplayHeight(dpy, screen)) * 25.4)
			/ ((double) XDisplayHeightMM(dpy, screen)));
	return std::pair<int, int>(horizontalDPI, verticalDPI);
}

uint64_t getMonotonicTimeNanoseconds() {
	struct timespec current;
	clock_gettime(CLOCK_MONOTONIC, &current);
	return ((current.tv_sec * 1000000000ULL) + (current.tv_nsec));
}

std::vector<std::string> &split(const std::string &s, char delim,
		std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

KeyCombo stringToKeyCombo(std::string str, Display* display) {
	KeyCombo combo;
	combo.keySym = 0;
	combo.mask = 0U;
	if (str.empty()) {
		combo.keySym = XK_VoidSymbol;
		return combo;
	}
	bool hasPrimaryKey = false; // If no key is specified, produce an error
	for (auto token : split(str, '+')) {
		if (token == "Shift") {
			combo.mask |= ShiftMask;
		} else if (token == "Ctrl") {
			combo.mask |= ControlMask;
		} else if (token == "Alt") {
			combo.mask |= Mod1Mask;
		} else {
			if (hasPrimaryKey)
				throw new std::runtime_error(
						"Invalid key combination: two primary keys!");
			combo.keySym = XStringToKeysym(token.c_str());
			if (combo.keySym == NoSymbol)
				throw new std::runtime_error(
						"Invalid key combination: invalid key!");
			combo.keyCode = XKeysymToKeycode(display, combo.keySym);
			hasPrimaryKey = true;
		}
	}
	if (!hasPrimaryKey)
		throw new std::runtime_error(
				"Invalid key combination: no primary key specified!");
	return combo;
}

bool keyComboMatches(KeyCombo combo, XKeyEvent* event) {
	return ((event->state & (ShiftMask | ControlMask | Mod1Mask)) == combo.mask)
			&& (event->keycode == combo.keyCode);
}

boost::format glxosdFormat(const std::string& str) {
	boost::format formatter(str);
	formatter.exceptions(
			boost::io::all_error_bits ^ (boost::io::too_many_args_bit));
	return formatter;
}

}
