/*
 * Copyright (C) 2013-2014 Nick Guletskii
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef LOCK_HPP_
#define LOCK_HPP_
#include <pthread.h>
#include <X11/Xlib.h>

class Lock {
public:
	Lock(pthread_mutex_t *mutex) :
			mutex(mutex), locked(true) {
		pthread_mutex_lock(mutex);
	}

	~Lock() {
		if (locked)
			pthread_mutex_unlock(mutex);
	}

	operator bool() const {
		return locked;
	}

	void unlock() {
		pthread_mutex_unlock(mutex);
		locked = false;
	}

private:
	pthread_mutex_t *mutex;
	bool locked;
};

#define PTHREADS_MUTEX(name) pthread_mutex_t name = PTHREAD_MUTEX_INITIALIZER;
#define PTHREADS_COND(name) pthread_cond_t name = PTHREAD_COND_INITIALIZER;

#endif /* LOCK_HPP_ */
