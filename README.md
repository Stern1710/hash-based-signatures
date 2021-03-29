# Hash Based Signature #

This is an extension to the [reference implemenetation](https://github.com/cisco/hash-sigs) of LMS signatures with additional functionality to bulk sign and verfiy whole directories and their subfolders, passed as a single argument instead of all the files individually.

## General info ##

For my bachelor thesis, the focus fell on hash-based signatures, in detail the Leighton-Micali Signatures (LMS) scheme which is explained in detail in [RFC 8554](https://tools.ietf.org/html/rfc8554). As quantum computing is on the rise, the now widely-used signature schemes my be suspectable to successfull attacks on their underlying asymmetric cryptographic functions. At the moment, most believed to be cryptograhically secure algorithm use mathematical problems such as factorisation or discrete logarithm which are hard to crack on the current computer systems. With clever use of quantum effects, Shor has described in [his groundbracking paper](https://ieeexplore.ieee.org/abstract/document/365700) a way to reduce the runtime complexity of the aforementioned mathematical problems from exponential to polynomial time.

Hash functions are not affected by Shor's algorithm and their time complexity may only be reduced by the the factor of two, which is the same for symmetric cyphers. Therefore a algorithm which at the moment provides a security of $2^{128}$ bits will have a security of $2^{64}$ bits against quantum computing attacks. In return, doubling the security and with this often just the key sizes is enough to keep the security up to secure standards.

Hash-based signatures are divided into the stateless and statefull schemes. Stateless offer advantages with typically simpler implementations and are easier to mange, but suffer from bigger signature sizes and decreased performance. For statefull signature schemes, the XMSS (Extended Merkle signature scheme) and LMS seem to be the most promising approaches. However, LMS is typically the faster method, therefore the decision fell on the usage of this scheme.

There already exists a reference implemenetation](https://github.com/cisco/hash-sigs)for LMS signatures which is the basis for this extension with new functions such as bulk signing. The features are usage are described in the according sections.

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
