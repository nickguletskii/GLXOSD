---
layout: default
title:  "FAQ"
description: "FAQ"
---
* TOC
{:toc}

### How do I customise GLXOSD? ###

Please see the configuration files:

* /etc/glxosd.conf for global configuration, contains the default config
* ~/.glxosd/glxosd.conf for user configuration

### How do I launch a Steam game with GLXOSD? ###

[How to use GLXOSD with Steam](/usage.html#steam-games)

### How do I launch a 32 bit application on a 64 bit OS with GLXOSD? ###

If you are running Ubuntu or Debian, you should install the 32 bit libraries through the package manager. You can do that by following the installation procedure described on the install page.


If you are not running Ubuntu or Debian, you will have to compile 32 bit binaries in a chroot and then copy them into the appropriate location (which can be distro specific) and edit the `glxsosd` launcher to contain the correct paths.

### How can I inject GLXOSD into an already running process? ###

There is no official way to do it yet, but you may have some luck with existing live shared object injection tools.

### When I launch an application with GLXOSD it crashes! Why? ###

If you find an application that does that, please file an issue on the [issue tracker](https://github.com/nickguletskii/GLXOSD/issues?state=open).

### Why doesn't GLXOSD display the temperatures of AMD GPUs? ###

I don't have an AMD GPU, so I can't write a plugin to interface with their drivers. Commits are welcome!

### Why doesn't GLXOSD display the temperature of my NVIDIA GPU? ###

Please make sure that the following conditions are met:
* The proprietary NVIDIA drivers are installed and active.
* The NVIDIA sensor support plugin for GLXOSD is installed (if you installed GLXOSD from packages).
* The NVIDIA driver API headers were found during compilation (if you built GLXOSD from source).
* Nothing is unsetting the GLXOSD_PLUGINS environment variable.
* The NVIDIA sensor support plugin for GLXOSD is being loaded by GLXOSD (check the terminal).

### Help! When I start an application, I get "undefined symbol: XextFindDisplay"! ###

I can't replicate this problem, and it might be caused by a misconfigured distro, so if you have any information, please post it [here](https://github.com/nickguletskii/GLXOSD/issues/5). Meanwhile, there is a workaround, which is described in the next paragraph.


Please add the libXext shared library to your LD_PRELOAD. For example, on Ubuntu you have to prefix the command with `LD_PRELOAD="/usr/lib/x86_64-linux-gnu/libXext.so.6:${LD_PRELOAD}"` like this: `LD_PRELOAD="/usr/lib/x86_64-linux-gnu/libXext.so.6:${LD_PRELOAD}" glxosd glxgears` If you are launching a 32 bit application, replace "x86_64-linux-gnu" with "i386-linux-gnu".

### How does it work? ###

GLXOSD works by intercepting glXSwapBuffers and glXDestroyContext calls. It uses the dynamic linker to override the GLX implementation of these methods and call them after it has finished rendering the overlay.

### Is it safe to use GLXOSD with anti-cheat software? ###

While its very unlikely that you will get banned from games for using GLXOSD, you should understand that there is a risk of that happening. Please read the documentation of the anti-cheat software that you are running.

For example, VAC (Valve Anti-Cheat) seems to operate on a blacklist rather than a whitelist and doesn't seem to ban people who use applications that function similarly to GLXOSD (for example: Mumble Overlay and RivaTuner OSD).

### What if I have a question that isn't on this list? ###

Please email me. My email address is [nickguletskii200@gmail.com](mailto:nickguletskii200@gmail.com)