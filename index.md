---
layout: full
title:  "GLXOSD, an OSD and benchmarking tool for Linux"
description: "GLXOSD is an on-screen display (OSD)/overlay for OpenGL applications running on Linux with X11. It can show FPS, the temperature of your CPU, and if you have an NVIDIA graphics card (with proprietary drivers), it will also show the temperature of the GPU. Also, it can log frame timings, which is useful for benchmarking."
---
<dfn style="font-style: normal;">GLXOSD</dfn> is an on-screen display (OSD)/overlay for OpenGL applications running on Linux with X11. It can show FPS, the temperature of your CPU, and if you have an NVIDIA graphics card (with proprietary drivers), it will also show the temperature of the GPU. Also, it can log frame timings, which is useful for benchmarking.

<p>
	<div style="background-color: #000000">
		<ul class="example-orbit" data-orbit data-options="timer_speed: 5000">
			<li>
					<p align="center"><img src="img/glxgears.png" alt="Screenshot of GLXOSD running inside glxgears" class="text-center" ></img></p>
					<div class="orbit-caption">GLXOSD displaying info in glxgears.</div>
			</li>
			<li>
					<p align="center"><img src="img/hl2.png" alt="Screenshot of GLXOSD running inside glxgears"></img></p>
					<div class="orbit-caption">GLXOSD displaying info in Half-Life 2.</br><small style="color: #888888">This is a screenshot of a copyrighted video game. It is believed that screenshots may be exhibited under fair use. Half-Life 2 is a trademark of Valve Corporation.</small></div>
			</li>
		</ul>
	</div>
</p>
<p>
	<a href="install.html" class="button">Install</a>
	<a href="usage.html" class="button">How to use</a>
</p>

<div class="row">
	<div class="large-3 columns hide-for-medium-down">
		<svg xmlns="http://www.w3.org/2000/svg" width="100%" height="100%" viewBox="0 0 100 100">
			<text x="0" y="100" class=""
		        font-family="GeneralFoundicons" 
		        font-size="100">
		        &#xf00f;
		  	</text>
		</svg>
	</div>
	<div class="large-9 small-12 columns text-justify">
		<h1>Monitor</h1>
		<p>GLXOSD displays information in the top left corner of the application. It can show FPS, the temperature of your CPU, and if you have an NVIDIA graphics card (with proprietary drivers), it will also show the temperature of the GPU. This project aims to provide some of the functionality that RivaTuner OSD (which is used by MSI Afterburner) provides under Windows.</p>
	</div>
</div>
<hr>
<div class="row">
	<div class="large-9 small-12 columns text-justify">
		<h1>Benchmark</h1>
		<p>GLXOSD can log precisely when an application draws something onto the screen. This is useful for benchmarking because it allows you to graph the framerate, delays between frames, etc...</p>
		<p>These logs are called frame timing logs. Each line in one contains two numbers: the ID of the X11 drawable that was redrawn and the time in nanoseconds since the logging started. Please note that the overhead of logging is bigger than a nanosecond.</p>
	</div>
	<div class="large-3 columns hide-for-medium-down">
		<svg xmlns="http://www.w3.org/2000/svg" width="100%" height="100%" viewBox="0 0 100 100">
			<text x="0" y="100" class=""
		        font-family="GeneralFoundicons" 
		        font-size="100">
		        &#xf01a;
		  	</text>
		</svg>
	</div>
</div>
<hr>
<div class="row">
	<div class="large-3 columns hide-for-medium-down">
		<svg xmlns="http://www.w3.org/2000/svg" width="100%" height="100%" viewBox="0 0 100 100">
			<text x="0" y="100" class=""
		        font-family="GeneralFoundicons" 
		        font-size="100">
		        &#xf021;
		  	</text>
		</svg>
	</div>
	<div class="large-9 small-12 columns text-justify">
		<h1>Analyse</h1>
		<p>As GLXOSD frame timing logs are CSVs, you will have no trouble processing them with your tool of choice.</p>
		<p>There is also a <a href="{{site.baseurl}}/analyse.html">little web tool</a> that can calculate FPS, frame frequency, delays between frames and plot them.</p>
	</div>
</div>
<hr>

# Latest changes #

* 2014-08-22, version 2.3.2:
  * Stop GLXOSD from spamming stdout.

* 2014-08-20, version 2.3.1:
  * Use pthreads for mutexes and signaling.
  * Fix crashes on Arch Linux.
  * Add frame logging timed stop.
  * Add libsensors chip filter.

* 2014-08-13, version 2.2.2:
  * Fix blend functions not being reverted.
  * Fix GL_ELEMENT_ARRAY_BUFFER not being reverted.
  * Fix keyboard event state handling.
  * Move user config location to XDG compliant directory.
  * Add the ability of disabling key combos.
  * Now works on OpenGL 3.

* 2014-08-07, version 2.2.1:
  * Add the ability to log frame timings, which allows users to benchmark
  applications.
  * Add key bindings for toggling the OSD.

* 2014-07-05, version 2.1.2 (together with 2.1.1):
  * **BREAKING CHANGE!** Migrate to OpenGL 3 (adds support for OpenGL core profile). This also means that this project no longer requires FTGL. Also, the text looks better now.
  * Add support for configuring GLXOSD by specifying the path to the
     configuration file.
  * Version format change! Major.Minor-Patch -> Major.Minor.Patch

# Supported distros #

* You can compile the latest GLXOSD from source on any distro, but it will not have multiarch support. You may have to adjust some paths.
* The latest GLXOSD for Ubuntu 14.04 is available through the Launchpad PPA.
* An old version of GLXOSD (without Steam support) for Ubuntu 12.04, 12.10, 13.04 and 13.10 is available through the Launchpad PPA.

# Safety #

While its very unlikely that you will get banned from games for using GLXOSD, you should understand that there is a risk of that happening. Please read the documentation of the anti-cheat software that you are running.

For example, VAC (Valve Anti-Cheat) seems to operate on a blacklist rather than a whitelist and doesn't seem to ban people who use applications that function similarly to GLXOSD (for example: Mumble Overlay and RivaTuner OSD).

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.