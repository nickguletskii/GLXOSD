GLXOSD
=============
GLXOSD is a basic OSD that works by intercepting glXSwapBuffers calls, inspired from RivaTuner OSD on Windows.

![Screenshot of glxgears with GLXOSD](https://raw.github.com/nickguletskii/GLXOSD/master/screenshots/glxgears.png "Screenshot of glxgears with GLXOSD")

THIS PROJECT IS EXPERIMENTAL, USE IT AT YOUR OWN RISK!

Compiling
=============

Before compiling, you need to make sure that you have GCC and CMake installed. On Ubuntu, all you need to do is paste the following line into the terminal:

```
sudo apt-get install build-essential cmake
```

We also need to make sure that the required libraries and their headers are installed. This project requires OpenGL, GLU, GLX, FontConfig, Freetype, Boost, FTGL and libsensors. To install them on Ubuntu, execute this:

```
sudo apt-get install mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev libfontconfig1-dev libfreetype6-dev libsensors4-dev libboost-dev libftgl2 libftgl-dev
```

Then, compile:

```
cmake -G "Unix Makefiles"
make all
```

Installing
=============

```
sudo make install
```

Configuring
=============

You can change system-wide settings by editing "/etc/glxosd.conf". You can also create a folder called ".glxosd" in your home directory and a file called "glxosd.conf" in it to change user settings.

Using
=============

When launching an OpenGL application, prepend glxosd to the command. For example, to launch glxgears with the OSD you would do this:

```
glxosd glxgears
```

Known limitations
=============

*32 bit applications running under a 64 bit operating system can't be injected.
*Doesn't read AMD graphics card temperatures.

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
