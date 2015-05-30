GLXOSD
=============

[You should probably have a look at GLXOSD's website first!](http://glxosd.nickguletskii.com/)
-------------
GLXOSD is an on-screen display (OSD)/overlay for OpenGL applications running on Linux with X11. It can show FPS, the temperature of your CPU, and if you have an NVIDIA graphics card (with proprietary drivers), it will also show the temperature of the GPU. Also, it can log frame timings, which is useful for benchmarking. This project aims to provide some of the functionality that RivaTuner OSD (which is used by MSI Afterburner) provides under Windows.

![Screenshot of glxgears with GLXOSD](https://raw.github.com/nickguletskii/GLXOSD/master/screenshots/glxgears.png "Screenshot of GLXOSD in glxgears")

Compiling
=============

[You should probably have a look at the installation page on GLXOSD's website first!](http://glxosd.nickguletskii.com/install.html)

Before compiling, you need to make sure that you have GCC and CMake installed. On Ubuntu, all you need to do is paste the following line into the terminal:

```
sudo apt-get install build-essential cmake
```

We also need to make sure that the required libraries and their headers are installed. This project requires OpenGL, GLU, GLX, FontConfig, Freetype, Boost, and libsensors. To install them on Ubuntu, execute this:

```
sudo apt-get install mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev libfontconfig1-dev libfreetype6-dev libsensors4-dev libboost-dev
```

Then, compile:

```
cmake -G "Unix Makefiles"
make all
```

Installing
=============

[You should probably have a look at the installation page on GLXOSD's website first!](http://glxosd.nickguletskii.com/install.html)

To install GLXOSD, just execute

```
sudo make install
```

Configuring
=============

http://glxosd.nickguletskii.com/faq.html#how-do-i-customise-glxosd

Using
=============

When launching an OpenGL application, prepend glxosd to the command. For example, to launch glxgears with the OSD you would do this:

```
glxosd glxgears
```

Known limitations
=============

* Doesn't read AMD graphics card temperatures.

For developers
=============

CMake is pretty new to me and I am not very comfortable with C++ either, so the code isn't all nice and CMakeLists are a mess. The project lacks customizability and there isn't much error handling. Commits are welcome!

Project structure
=============

The src directory has a subdirectory for each module:

* elfhacks - a library of various ELF run-time hacks, cloned from https://github.com/nullkey/elfhacks
* glinject - the module that "injects" the OSD into each frame.
* glxosd - the OSD itself.

Licensing
=============

The LICENSE file contains the license which applies to everything except:

* Files located in the following directories:
	* src/elfhacks
* Binary builds of the files listed above.
* Files under the fonts/CPMono_v07 directory. Please read the Creative Commons license which can be found inside that directory.
