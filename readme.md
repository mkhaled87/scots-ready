# SCOTS: Automated Synthesis of Symbolic Controllers for General Nonlinear Systems [static CUDD-lib Version]

**SCOTS** is a C++ tool (with a small Matlab interface) to synthesize controller for
possibly perturbed nonlinear control systems with respect to safety and reachability specifications.

This version of SCOTS is forked from the [version in LRZ-Gitlab](https://gitlab.lrz.de/hcs/scots) with the following modifications:

- The CUDD library is included pre-configured for ease of installation and use,        
- The CUDD library is used as a static-link library,
- A Windows VisualSudio build project is provided for the CUDD library,
- Some minor fixes are applied to the CUDD libtary code and SCOT's code, and 
- a Docker installtion with the [BDDImplement](https://github.com/mkhaled87/BDD2Implement) tool included.

## Installation using Docker
Here, we assume you will be using a Linux or MacOS machine. Commands will be slightly different on Windows if you use Windows PowerShell.
First, make sure you have docker installed (see Docker installation guide for: [MacOS](https://docs.docker.com/docker-for-mac/install/), [Ubuntu Linux](https://docs.docker.com/engine/install/ubuntu/) or [Windows](https://docs.docker.com/docker-for-windows/install/)). Also, make sure to [configure Docker to use sufficient resources](https://docs.docker.com/config/containers/resource_constraints/) (e.g., enough CPU cores). Otherwise, OmegaThreads will run slower than expected.

Download the Dockerfile:
``` bash
$ mkdir scots-ready
$ cd scots-ready
$ curl https://raw.githubusercontent.com/mkhaled87/scots-ready/master/Dockerfile -o Dockerfile
```    

Build the Docker container (don't forget the DOT at the end):
``` bash
$ docker build -t scots/latest .
```    
The building process will take proximately 15 minutes. 
During the build, you may recieve some red-colored messages.
They do not indicate errors, unless you recieve an explicit red-colored error message.
Once done, run/enter the image's interactive shell:

``` bash
$ docker run -it -v ~/docker_shared:/docker_shared scots/latest
```

Note that by the previous command, we made a pipe between host and the container (this part: *-v ~/docker_shared:/docker_shared*) which will later you to move files (e.g., the synthesized controller) from the container to the host.

Now SCOTS is installed in (**/scots-ready/**).
You may navigate to it:

``` bash
/# cd scots
```

Now, you may continue to the **Quick Start** section below to run a simple examnple.

## Installation (non Docker)

SCOTS requires only a modern C++ development environment where you can compile C++ (v11) source codes.
All other requirements are included with SCOTS.

To make full use of SCOTS, you may need to have MATLAB installed to be able to simulate the synthesized controllers using the provided MATLAB interface in [/mfiles](/mfiles).

SCOTS was originally developed to work in Linux and MacOS. 
However, we provide a small guide below on how to install it also in Windows. 
SCOTS is known to work much slower in Windows.

### Installation using Linux/MacOS

We use the *g++* compiler and *clang++* compiler frontend. If you dont have them installed, please install them first.
If you are using Ubuntu Linux, you may install the compiler and required libraries using the commands:

    $ sudo apt-get install git build-essential gcc g++ clang
    
If you howver using MacOS, installing XCode frorm Apple Store will be enough.
Now, clone this repisatory and move to its root:

    $ git clone https://github.com/mkhaled87/scots-ready.git ./scots
    $ cd scots

Now, we build the included CUDD library by running the command:

    $ make

This will build the included CUDD library as a static-link library.

### Installation using (Windows)

You may use [Windows Subsystem For Linux (WSL)](https://docs.microsoft.com/en-us/windows/wsl/install-win10) and follow the same instructions before to run SCOTS. However, the MATLAB interface is not be supported.

Another option is to use the provided [VisualStudio build project](cudd-3.0.0/cudd.sln) provided with the CUDD library to build a static version of the library. After building, the static library will be loccacted in *cudd-3.0.0/lib*. You may then make your own VisualStudio projects from any example source code and link against the library.

## MATLAB Interface Installation

If you don't have matlab or plan to use it later, skip this section for now.
The matlab interface allows loading the output files of SCOTS inside matlab and simulating the closed loop of the control system.

Open MATLAB and navigate to the folder *mfiles*.
Use the command window of MATLAB and run the command:

	>> Install

This all build/install the MATLAB interface and will also add the *mfiles* folder and all subfolders to MATLAB's path.
This should work in Linux, MacOS and Windows (not WSL).

## Quick Start

Now, we test scots with one of the provided examples.
Navigate, build and run the hscc16/vehicle1 example:

    $ cd examples/hscc16/vehicle1
    $ make
    $ ./vehicle
	
If you are using Docker, you are limited to the previous step where you generated the controller.
A simulation using MATLAB from inside Docker is currently not supportef.
Continue if you installed SCOTS directly not using Docker.
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
