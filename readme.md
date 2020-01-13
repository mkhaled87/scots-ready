# SCOTS: Automated Synthesis of Symbolic Controllers for General Non-lineal Systems [static CUDD-lib Version]

**SCOTS** is a C++ tool (with a small Matlab interface) to synthesize controller for
possibly perturbed nonlinear control systems with respect to safety and reachability specifications.

This version of SCOTS is forked from the [version in LRZ-Gitlab](https://gitlab.lrz.de/hcs/scots) with the following modifications:

1- The CUDD library is included pre-configured for ease of installation and use,        
2- The CUDD library is used as a static-link library,
3- A Windows VisualSudio build project is provided for the CUDD library, and
4- Some minor fixes are applied to the CUDD libtary code and SCOT's code.

## Requirements

SCOTS requires only a modern C++ development environment where you can compile C++ (v11) source codes.
All other requirements are included with SCOTS.

To make full use of SCOTS, you may need to have MATLAB installed to be able to simulate the synthesized controllers using the provided MATLAB interface in [/mfiles](/mfiles).

SCOTS was originally developed to work in Linux and MacOS. However, we managed to make it work in Windows and included a small help to guide you [here](/installation_notes_windows.txt). SCOTS is known to work much slower in Windows.

## Installation (Linux/MacOS)

In most of the examples, we use the *g++* compiler and *clang++* compiler frontend. If you dont have them installed, please install them first.
If you are using Ubuntu Linux, you may install the compiler and required libraries using the commands:

    $ sudo apt-get install git build-essential gcc g++ clang
    
Then, clone this repisatory and move to its root:

    $ git clone https://github.com/mkhaled87/scots-ready.git
    $ cd scots-ready

Now, we build the included CUDD library by running the command:

    $ make

This will build the included CUDD library as a static-link library.

SCOTS itself is a header only library. You only need to add SCOTS source directory to the include directory in the compiler command when you work with SCOTS. However, SCOTS depends on the CUDD library to represent the data structures used in SCOTS.

Further details are found in the readme files in example directories and in the [manual](/manual/manual.pdf).

## Installation (Windows)

You may use [Windows Subsystem For Linux (WSL)](https://docs.microsoft.com/en-us/windows/wsl/install-win10) and follow the same instructions before to run SCOTS. However, the MATLAB interface is not be supported.

Another option is to use the provided [VisualStudio build project](cudd-3.0.0/cudd.sln) provided with the CUDD library to build a static version of the library. After building, the static library will be loccacted in *cudd-3.0.0/lib*. You may then make your own VisualStudio projects from any example source code and link against the library.

## MATLAB Interface Installation

If you don't have matlab or plan to use it later, skip this section for now.
The matlab interface allows loading the output files of SCOTS inside matlab and simulating the closed loop of the control system.

Open MATLAB and navigate to the folder *mfiles*.
Use the command window of MATLAB and run the command:

	>> Install

Then, add the *mfiles* folder and all subfolders to MATLAB's path.
This should work in Linux, MacOS and Windows (not WSL).

## Quick Start

Now, we test scots with one of the provided examples.
Navigate, build and run the hscc16/vehicle1 example:

    $ cd examples/hscc16/vehicle1
    $ make
    $ ./vehicle
	
Now, form inside MATLAB, navigate to the examples folder *examples/hscc16/vehicle1* and simulate the closed loop using the command:

	>> vehicle

This should simulate the colsed loop of the autonomous vehicle and show a result similar to the following:

![vehicle_sim_out](manual/sim_vehicle.png?raw=true)

## Directory structure

- **./bdd/**:

    Contains the source C++ source code for the SCOTS classes which use Binary Decision Diagrams as underlying data structure.

- **./doc/**:

    C++ Class documentation directory.
  
- **./examples/**:

    Some C++/Maltab programs demonstrating the usage of SCOTS.
  
- **./manual/**:

    Contains a the manuel with its tex source.
  
- **./mfiles/**:

    Contains an mfile as a wrapper to the mex-file functions.
  
- **./mfiles/mexfiles/**:

    mex-file to read the C++ output from file.

## Support

Please report any problems/bugs you face while installing and running SCOTS to [Mahmoud Khaled](http://hyconsys.com/members/mkhaled/).
