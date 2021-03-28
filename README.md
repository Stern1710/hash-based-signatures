# hash-based-signatures
Based on the [reference implemenetation](https://github.com/cisco/hash-sigs) for LMS signatures, this is my own addition for bulk signing whole directories as part of my bachelor thesis

## General info ##

## Technologies ##

The reference implementation is written in C, therefore these modifications are also using the C language. The makefile is a modified version of the original one to take the made changes into account.

## Setup ##

First of all, clone this repository. The easiest way to compile the program is to just run a `make all` which takes care of everything. Most Linux distributions have already preinstalled this little program or offer it in their repository, for Windows this task is also possible, but can be more of a challange. Programs like `GnuWin32` or as a package from `Chocolate`.

As this is a C program, a compiler like `gcc` has to be installed as well. I personally used `gcc` in the most recent version at the time of writing this readme.

The code also relies on the hash function from the openssl libraries. Out of the box, most operating system do not provide the sources for this. Most linux distributions provide the openssl header files in the package `openssl-dev` or `openssl-devel`, under Ubuntu/Debian it should be `libssl-dev`.

## Features ##

The program will provide all the features from the reference implementation and adds some extensions to this. For signing and verifying, folders (including subfolders) can be done in one pass by just passing the folder as a single argument. Also the output will be reformated and only done after all signing/verification operations are completed to reduce overhead by avoiding a lot of smaller output operations to the standard output.

## Usage ##

To be done once the syntax is clear

## Credits ##

Many thanks to my supervisor a.Univ.-Prof, Dr. Josef Scharinger for making this all possible and providing me with great ideas and scientific literature.

## Contact ##

In case one got questions about my modifications to the source code, please drop me a mail to [k11812499@students.jku.at](mailto:k11812499@students.jku.at).
