Processing-Terminal
===================

A Terminal (ASCII Art) [Processing](http://processing.org/) Interpreter. 

### Requirements
Tested on Linux (Ubuntu/Debian based distributions). Mac OS X and Windows (Cygwin/MINGW) should be possible.

- libCaca/ncurses
- Perl (File::Slup and File::copy Modules)
- GNU C++ Compiler

Installing requirements on Ubuntu/Debian:

    sudo apt-get install libfile-slurp-perl libcaca0 libcaca-dev git ncurses


###Installation
Get Processing-Terminal from Github

    git clone https://github.com/rlfbckr/processing-terminal

Add processing-terminal to your PATH or add an alias in **.bashrc**

    alias processing-terminal="/path/to/processing-terminal/src"      
   

### How it works
Processing-Terminal consists of two parts:

1.  C/C++ framework that mimics the Processing-core functions (e.g. draw(), line(), rect(), map())
2.  A Perl-Script that builds and runs the processing-code (similar to Arduino)

The Perl-Script **processing-terminal** does the following:

    processing-terminal demo.pde

1.  It reads the given **.pde** file
2.  It adds an **#include** of the **processing-terminal.h** header file to the first line
3.  Precompiles the **processing code** 
    -   generating **C** function headers
    -   inserts missing **setup()** and **draw()** functions
    -   translates **java** syntax to **C** syntax
4.  Adds an a **main** function
5.  Builds and executable of the precompiled **C** code
    -   create a **build** directory in the current path
    -   copys the required C files and headers into the build directory
    -   builds the executable with g++:
6.  **Executes** the build
7.  Restores the **terminal** (stty sane)

An example build command looks like this

    g++ -Wall  -Wunused-but-set-variable -g  -o build/demo build/processing-terminal.c build/demo.c -Wall -lcaca -lpthread -Wunused-but-set-variable
