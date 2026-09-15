# autoc

A WIP build tool currently designed to work with small and medium-sized C and C++ projects **on Linux**

## Overview

This tool is designed to be installed once on your system and be easily configured for each project through a "autoc.conf" file.

As mentioned previously, it's currently a work in progress and currently it can only compile and link projects.

### NOTE

**This tool is very early in development and currently not quite useful as intended**

### Motivation

I have a general-purpose makefile (repository [here](https://github.com/EcologicPack34/general-makefile)), but the main problem I've encountered for my needs is that it can't mix multiple source file extensions without adding a lot of extra rules.

Why use this over other tools like CMake or Ninja? Simply **don't**. This is mainly a personal project to learn C++ and doesn't try to replace more advanced projects (nor that I could do that) and it's currently very early in development.

## Characteristics

- Project compilation and linking to an executable.

- Support for multiple source files extensions (for example: .c and .cpp in a C++ project)

- Support for subfolders inside the specified include and source directories.

- Shell free compiler invocation.

## Future plans

- Full parity with the general-purpose makefile previously mentioned (repository [here](https://github.com/EcologicPack34/general-makefile)).

- More modular design and implementation for later development.

- Parallel source file compilation.

- Versioning system with support for release and debug builds.

- Proper tests.

- Support for other project types such as libraries instead of executables only.

- Maybe and just maybe, Windows support.

## Project Structure

<pre>
.
├── include/   # Header files
├── src/       # Source files
├── obj/       # Object files
├── build/     # Executable output
├── autoc.conf # Testing configuration file
└── makefile   # Project makefile for building
</pre>


  
