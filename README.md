GLXOSD
=============
GLXOSD is a basic OSD that works by intercepting glXSwapBuffers calls, inspired from RivaTuner OSD on Windows. For now the OSD only draws the FPS, but I have plans to add sensor information to the OSD as well.

THIS PROJECT IS EXPERIMENTAL, USE IT AT YOUR OWN RISK!

Compiling
=============

Before compiling, you need to make sure that you have GCC and CMake installed. On Ubuntu, all you need to do is paste the following line into the terminal:

```
sudo apt-get install build-essential cmake
```

We also need to make sure that the required libraries and their headers are installed. This project requires OpenGL, GLU, GLX, FontConfig and Freetype. To install them on Ubuntu, execute this:

```
sudo apt-get install mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev libfontconfig1-dev libfreetype6-dev
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

Using
=============

When launching an OpenGL application, prepend glxosd to the command. For example, to launch glxgears with the OSD you would do this:

```
glxosd glxgears
```

Known limitations
=============

32 bit applications running under a 64 bit operating system can't be injected.

For developers
=============

CMake is pretty new to me and I am not very comfortable with C++ either, so the code isn't all nice and CMakeLists are a mess. The project lacks customizability and there isn't much error handling. Commits are welcome!

Project structure
=============

The src directory has a subdirectory for each module:

* elfhacks - a library of various ELF run-time hacks, cloned from https://github.com/nullkey/elfhacks
* ftgl - a text rendering library. http://sourceforge.net/apps/mediawiki/ftgl/index.php?title=Main_Page
* glinject - the module that "injects" the OSD into each frame.
* glxosd - the OSD itself.

Licensing
=============

The LICENSE file contains the license which applies to everything except:

* Files located in the following directories:
	* src/elfhacks
	* src/ftgl
* Binary builds of the files listed above.
* The file fonts/Square.ttf, which is available for "free" from http://www.dafont.com/squarefont.font
