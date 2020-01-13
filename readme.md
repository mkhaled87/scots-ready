# SCOTS: Automated Synthesis of Symbolic Controllers for General Non-lineal Systems [static CUDD-lib Version]

**SCOTS** is a C++ tool (with a small Matlab interface) to synthesize controller for
possibly perturbed nonlinear control systems with respect to safety and reachability specifications.

This version is the same version of SCOTS as in here but with the CUDD library included and pre-configured for ease of installation and use.

## Requirements

SCOTS requires only a modern C++ development environment where you can compile C++ (v11) source codes.
All other requirements are included with SCOTS.

To make full use of SCOTS, you may need to have MATLAB installed to be able to simulate the synthesized controllers using the provided MATLAB interface in [/mfiles](/mfiles).

SCOTS was originally developed to work in Linux and MacOS. However, we managed to make it work in Windows and included a small help to guide you [here](/installation_notes_windows.txt). SCOTS is known to work much slower in Windows.

## Installation

First, we build the included CUDD library by navigating to SCOTS root folder and running the command:

    $ make

This will build the included CUDD library as a static-link library.

SCOTS itself is a header only library. You only need to add SCOTS source directory to the include directory in the compiler command when you work with SCOTS. However, SCOTS depends on the CUDD library to represent the data structures used in SCOTS.

Further details are found in the readme files in example directories and in the [manual](/manual/manual.pdf).

For installing and running SCOTS on Windows, please refer to the [Windows installation help file](/installation_notes_windows.txt).

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
