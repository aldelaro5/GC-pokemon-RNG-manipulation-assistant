# GC-pokemon-RNG-manipulation-assistant
![Screenshot](https://raw.githubusercontent.com/aldelaro5/GC-pokemon-RNG-manipulation-assistant/master/Docs/screenshot.png)

[![Travis CI build Status](https://travis-ci.org/aldelaro5/GC-pokemon-RNG-manipulation-assistant.svg?branch=master)](https://travis-ci.org/aldelaro5/GC-pokemon-RNG-manipulation-assistant)
[![AppVeyor build status](https://ci.appveyor.com/api/projects/status/3k6v4h3rxl4pjd57/branch/master?svg=true)](https://ci.appveyor.com/project/aldelaro5/gc-pokemon-rng-manipulation-assistant/branch/master)

A program to aid RTA speedrunners to manipulate the starters in Pokemon Colosseum and Pokemon XD: Gale of Darkness. This manipulation involves using the random battle feature of each game to find the current seed and hitting the right frame when confirming the trainer name to get the desired starters RNG. 

This program allows this manipulation technique to work as it assists the runner into all these steps with user-friendliness being a priority. The goal of having such a method is to reduce the amount of early resets required to start a run and to also open up new possibilities for having better than usual starters as this method makes it easier to see the different possibilities.

For binary releases of this program, refer to [the "releases" page](https://github.com/aldelaro5/GC-pokemon-RNG-manipulation-assistant/releases) on [the Github repository](https://github.com/aldelaro5/GC-pokemon-RNG-manipulation-assistant).

## System requirements
Any x86_64 based system should work. For Windows, you must have the [Microsoft Visual C++ Redistributable for Visual Studio 2017](https://support.microsoft.com/en-ca/help/2977003/the-latest-supported-visual-c-downloads) installed.

For faster results, a CPU with high parallelism capabilities is recommended. This means that this program will benefit from having a CPU that has multiple cores/threads, especially if you are generating a precalculation file. Additionally, 500MB of free RAM is recommended (1.5GB if you are generating a precalculation file).

## General usage
Download the precalculation file provided along the binaries release for the game you intend to use the manipulation and extract it into the program's directory. Then, select the game in the dropdown list and start the seed finding procedure by clicking on `Find your seed`. This will show a wizard to guide you through the process. If it succeeds, you will be able to see your starters prodictions depending on the amount of frames it takes to press A on the new game confirmation prompt and pressing A on the name confirmation prompt. Alternatively, you may set the seed manually by entering the seed in hexadecimal in the appropriate field and click `Set Seed`.

You can set the desired predictions you wish to get by configuring the filters in the settings and you can also reroll the predictions as many times as you want providing you generate another random battle team. You may also store the current seed to restore it later on if you wish to see if better options are available with more rerolls.

Once you are satisfied with a certain prediction, you are ready to start your run, but for the manipulation to succeed, you have to hit the right frame when confirming the trainer name which is a time span of 1/60th of a second. A program like [FlowTimer](https://github.com/stringflow/FlowTimer/releases) or [Eon timer](https://bitbucket.org/ToastPlusOne/eontimer/downloads/) can assist you by providing audio and visual cues for the timing. You may quickly verify the success of the manipulation by comparing the trainer ID of the starter with the trainer ID of the prediction you wished to get as well as the HP stat displayed next to the HP IV.

Alternatively, you may generate the precalculation file for the game yourself in the `File` menu, but this process will take a few hours depending on your CPU and threads count. For this reason, it is highly recommended to download the file instead since the file is the same for every usage of the program.

Finally, it is possible to have a pretty report of the stats of the starters as well as a way to predict the stats of the secondaries Pokémon using the `Stats reporter` part of the interface. You must select the row coresponding to the seed you wish to use during the run before using the stats reporter.

## How to Build
### Microsoft Windows
This repository provides a solution file for Visual Studio 2015 and later. The Windows SDK Version 10.0.16299.0 (Windows 10 Fall Creators Update) is required.

Before proceeding, ensure the Qt submodule is initialized by running `git submodule update --init` at the repository's root. The files should appear at the `Externals\Qt` directory.

Once complete, open the solution file `Dolphin-memory-engine.sln` located in the `Source` directory with Visual Studio. Select the build configuration and build it.

#### Windows SDK
The Windows SDK version 10.0.16299.0 comes with the C++ Desktop Development Workload of Visual Studio 2017 — other versions may work but are untested. To use a different Windows SDK you'll need to select it in the project properties window. Please note that this will change the `vcxproj` file, so if you plan to submit a Pull Request, make sure to not stage this change.

### Mac OS
> _The Homebrew package manager must be installed, [refer to the project's website](https://brew.sh/) for more information._

The CMake, Qt and libomp packages from homebrew must be installed. To install them, run the following commands into a terminal:

	brew install cmake
	brew install qt
	brew install libomp

Additionally, for the build system to properly recognise Qt, some environements variables needs to be modified accordingly. One can do so by appending these lines to the ~/.bash_profile file:

	export PATH="/usr/local/opt/qt/bin:$PATH"
	export LDFLAGS="-L/usr/local/opt/qt/lib"
	export CPPFLAGS="-I/usr/local/opt/qt/include"

Once this is done, simply run the following commands from the `Source` directory to start the build process:

	mkdir build && cd build
	cmake ..
	make

An application bundle will be built in the directory named `build`.

### Linux
> _CMake and Qt 5 are required. Please refer to your distribution's documentation for specific instructions on how to install them._

To build, simply run the following commands from the `Source` directory:

	mkdir build && cd build
	cmake ..
	make

The compiled binaries should appear in the directory named `build`.

## License
This program is licensed under the MIT license which grants you the permission to do anything you wish to with the software, as long as you preserve all copyright notices. (See the file LICENSE for the legal text.)
