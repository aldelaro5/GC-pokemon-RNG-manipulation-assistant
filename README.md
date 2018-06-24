# GC-pokemon-RNG-manipulation-assistant
![Screenshot](https://raw.githubusercontent.com/GC-pokemon-RNG-manipulation-assistant/master/Docs/screenshot.png)

[![Travis CI build Status](https://travis-ci.org/aldelaro5/GC-pokemon-RNG-manipulation-assistant.svg?branch=master)](https://travis-ci.org/aldelaro5/GC-pokemon-RNG-manipulation-assistant)
[![AppVeyor build status](https://ci.appveyor.com/api/projects/status/3k6v4h3rxl4pjd57/branch/master?svg=true)](https://ci.appveyor.com/project/aldelaro5/gc-pokemon-rng-manipulation-assistant/branch/master)

A program to aid RTA speedrunners to manipulate the starters in Pokemon Colosseum and Pokemon XD: Gale of Darkness. This manipulation involves booting the console after setting its clock, using the random battle feature of each game to find the current seed and hitting the right frame when confirming the trainer name to get the desired starters RNG. 

This program allows this manipulation technique to work as it assists the runner into all these steps with user-friendlyness being a priority. The goal of having such a method is to reduce the amount of early resets required to start a run and to also open up new possibilities for having better than usual starters as this method makes it easier to see the different possibilities.

For binary releases of this program, refer to [the "releases" page](https://github.com/aldelaro5/Dolphin-memory-engine/releases) on [the Github repository](https://github.com/aldelaro5/Dolphin-memory-engine).

## System requirements
Any x86_64 based system should work, however, Mac OS will require the use of [Wine](https://www.winehq.org/) as I do not have the necessary tools to build for this OS.

For faster results, a CPU with high parallelism capabilities is recommended. This means that this program will benefit _greatly_ from having a CPU that has multiple cores/hreads.

## How to Build
### Microsoft Windows
This repository provides a solution file for Visual Studio 2015 and later. The Windows SDK Version 10.0.16299.0 (Windows 10 Fall Creators Update) is required*.

Before proceeding, ensure the Qt submodule is initialized by running `git submodule update --init` at the repository's root. The files should appear at the `Externals\Qt` directory.

Once complete, open the solution file `Dolphin-memory-engine.sln` located in the `Source` directory with Visual Studio. Select the build configuration and build it.

#### Windows SDK
The Windows SDK version 10.0.16299.0 comes with the C++ Desktop Development Workload of Visual Studio 2017 — other versions may work but are untested. To use a different Windows SDK you'll need to select the it in the project properties window. Please note that this will change the `vcxproj` file, so if you plan to submit a Pull Request, make sure to not stage this change.

### Linux
> _CMake and Qt 5 are required. Please refer to your distribution's documentation for specific instructions on how to install them._

To build, simply run the following commands from the `Source` directory:

	mkdir build && cd build
	cmake ..
	make

The compiled binaries should be appear in the directory named `build`.

## General usage
First, select a game in the dropdown list. Then, start the seed finding procedure by clicking on "Find your seed". This will show a wizard to guide you through the process. If it succeeds, you will be able to see your starters prodictions depending on the amount of frames it takes to press A on the new game confirmation prompt and pressing A on the name confirmation prompt. You can set the desired predictions you wish to get by configuring the filters in the settings and you can also reroll the predictions as many times as you want providing you generate another random battle team. 

Once you are satisfied with a certain prediction, you are ready to start your run, but for the manipulation to suceed, you have to hit the right frame when confirming the trainer name which is a time span of 1/60th of a second. A program like the [Eon timer](https://bitbucket.org/ToastPlusOne/eontimer/downloads/) can assist you by providing audio and visual cues for the timing. You may quickly verify the success of the manipulation by comparing the trainer ID of the starter with the trainer ID of the prediction you wished to get.

## License
This program is licensed under the MIT license which grants you the permission to do  anything you wish to with the software, as long as you preserve all copyright notices. (See the file LICENSE for the legal text.)
