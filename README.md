# Hash Based Signature #

This is an extension to the [reference implemenetation](https://github.com/cisco/hash-sigs) of LMS signatures with additional functionality to bulk sign and verfiy whole directories and their subfolders, passed as a single argument instead of all the files individually.

## General info ##

For my bachelor thesis, the focus fell on hash-based signatures, in detail the Leighton-Micali Signatures (LMS) scheme which is explained in detail in [RFC 8554](https://tools.ietf.org/html/rfc8554). As quantum computing is on the rise, the now widely-used signature schemes my be suspectable to successfull attacks on their underlying asymmetric cryptographic functions. At the moment, most believed to be cryptograhically secure algorithm use mathematical problems such as factorisation or discrete logarithm which are hard to crack on the current computer systems. With clever use of quantum effects, Shor has described in [his groundbracking paper](https://ieeexplore.ieee.org/abstract/document/365700) a way to reduce the runtime complexity of the aforementioned mathematical problems from exponential to polynomial time.

Hash functions are not affected by Shor's algorithm and their time complexity may only be reduced by the the factor of two, which is the same for symmetric cyphers. Therefore a algorithm which at the moment provides a security of 2^128 bits will have a security of 2^64 bits against quantum computing attacks. In return, doubling the security and with this often just the key sizes is enough to keep the security up to secure standards.

Hash-based signatures are divided into the stateless and statefull schemes. Stateless offer advantages with typically simpler implementations and are easier to mange, but suffer from bigger signature sizes and decreased performance. For statefull signature schemes, the XMSS (Extended Merkle signature scheme) and LMS seem to be the most promising approaches. However, LMS is typically the faster method, therefore the decision fell on the usage of this scheme.

There already exists a [reference implemenetation](https://github.com/cisco/hash-sigs)for LMS signatures which is the basis for this extension with new functions such as bulk signing. The features are usage are described in the according sections.

## Technologies ##

The reference implementation is written in C, therefore these modifications are also using the C language. The makefile is a modified version of the original one to take the made changes into account.

## Setup ##

First of all, clone this repository. The easiest way to compile the program is to just run a `make all` which takes care of everything. Most Linux distributions have already preinstalled this little program or offer it in their repository, for Windows this task is also possible, but can be more of a challange. Programs like `GnuWin32` or as a package from `Chocolate`.

As this is a C program, a compiler like `gcc` has to be installed as well. I personally used `gcc` in the most recent version at the time of writing this readme.

The code also relies on the hash function from the openssl libraries. Out of the box, most operating system do not provide the sources for this. Most linux distributions provide the openssl header files in the package `openssl-dev` or `openssl-devel`, under Ubuntu/Debian it should be `libssl-dev`.

## Features ##

The program will provide all the features from the reference implementation and adds some extensions to this. For signing and verifying, folders (including subfolders) can be done in one pass by just passing the folder as a single argument. Also the output will be reformated and only done after all signing/verification operations are completed to reduce overhead by avoiding a lot of smaller output operations to the standard output. The reference implementation offers the key generation, if needed with specialized parameter sets, as well as signing and verifying individual files and advancing the key.

## Usage ##

```text
signer <command> [parameters]
```

The programm is very easy to use and each command follows a very simple pattern. The first argument is always the chosen option which can either be generating a key, signing, verifying or advancing the key. The second argument is the keyname, either for generating or using it. At the third position come either the files/folder to work with, the amount to advance they key or the paramter set for generating the key.

Sections of the syntax noted with `<>` have to be there, sections with `[]` are optional. However it should be stated that for files, at least a single file has to be passed as no file to sign/verify does not make sense, passing more than one file is of course possible and encouraged as no the loading of the private key also takes some time. Plese note that command and optional parameter do *not* need the brackets around them.

### Key generation ###

```text
signer genkey <keyname> [parameter set]
```

Generates a key with the given name. Usually, the private key, public key and an auxiliary file are created with resonable default values. By default, a Merkle trees with two levels is used. Top tree has 20 levels, bottom tree has 10 which is notaed as `20/10`. The Winternitz is set to 8 for both trees; this minimizes the signature size, gives us a reasonable genkey time (3 minutes with threading), good load times and a billion signatures per key.

The parameter set is in the form of `h0/w0,h1/w1:aux_size` like `15/4,10/8:2000`, where `h0` and `h1` denote the heights, `w0` and `w1` the Winternitz parameter and `aux_size` the maximum size of the auxdata file. Winternitz and auxiliary can be obmitted, in this case default values will be used.

Moreover, a custom `seed` (usually 32 byte hex-notation)  and `i` (16 byte hex-notation) value can be set which will be used for the top level tree, where `i` denotes the qunite identifier for the public/private key pair and `seed` is used as additional random data during the hashing process to increase security. However, this should only be done for specialized testing purposes, not for real world use as this could make forgeries far more likely to be plausible. Therefore it is highly discourage to set `seed`and `i` yourself. Setting would be done like `param=value`, i.e. `seed=0123456789abcdef i=fedcba98765432`.

### Signing ###

```text
signer sign <keyname> [files to sign]
```

```text
signer sign-bulk <keyname> <folder to sign>
```

Signing one or more files can be done by either specifying all the files individually which should be signed with to the `sign` command or just pass a directory to the `sign-bulk` command. A single folder or one or more files can be passed to the respective command.

### Verification ###

```text
signer verify <keyname> [files to verify]
```

```text
signer verify-bulk <keyname> <folder to verify>
```

Verfifying one or more files can be done by either specifying all the files individually which should be signed with to the `verify` command or just pass a directory to the `verify-bulk` command. A single folder or one or more files can be passed to the respective command.

### Key advancing ###

```text
signer advance <keyname> <amount of advance>
```

Advances the key with a certain amount, i.e. marking a given number of keys as unavailable for future signing operations. This behaves like signing an given amount of messages, but obviously without the actual signing operation.

### Example usage ###

First off, create the new key pair with the name `myKeys` and the parameter set `15/4,10/8:2000`

```text
signer genkey myKeys 15/4,10/8:2000
```

Use the generated key pair to sign two messages called `myTestFile1` and `myTestFile2`.

```text
signer sign myKeys path/to/myTestFile1 path/to/myTestFile2
```

Use the generated key pair to sign all files in a directory `myTestDir`

```text
signer sign-bulk myKeys path/to/myTestDir
```

Verify `myTestFile1` and `myTestFile2`

```text
signer verify myKeys path/to/myTestFile1 path/to/myTestFile2
```

Verify `myTestDir`

```text
signer verify-bulk myKeys path/to/myTestDir
```

Advance the keypair by the amount of three, therefore making the next three private keys unavailable for signing messages as the statefull scheme thinks they were already used.

```text
signer advance myKeys 3
```

## Tested parameter sets ##

The following parameter sets have been tested with the current state of the implementation using the following parameters. The signature sizes are the ones meassured on the system, the meassured time is taken from a Ryzen 5 mobile processing unit. For the hash function SHA-256 was used, therefore n equals to 32. We ignored the Winternitz parameter of w=1 deliberatly as this extreme settings seems very unlikely to us to be used due to the very large signature sizes where performance can be gained from using other tricks such as advancing the key, generating trees on demand, etc.

| Type    | h(:h)   | w     | aux   |  KeyGenTime  | SigTime    |  SigSize |
| ------- |:-------:| -----:| -----:|   -----: |       -----:|      -----:|
| LMS     | 20      | 2     |         |             |             |
| LMS     | 20      | 8     |         |             |             |
| HSS     | 20:10   | 2     |         |             |             |
| HSS     | 20:10   | 4     |         |             |             |
| HSS     | 20:10   | 8     |         |             |             |
| HSS     | 25:15   | 2     |         |             |             |
| HSS     | 25:15   | 4     |         |             |             |
| HSS     | 25:15   | 8     |         |             |             |

## Credits ##

Many thanks to my supervisor a.Univ.-Prof, Dr. Josef Scharinger for making this all possible and providing me with great ideas and scientific literature.

Many thanks to the authors of the often mentioned reference implementation for providing the code under the BSD-3-Clause License.

## Contact ##

In case one got questions about my modifications to the source code, please drop me a mail to [k11812499@students.jku.at](mailto:k11812499@students.jku.at).

## Licence ##

This program is, as the reference implementation, provided under the BSD-3-Clause License. The terms of the license can be seen in the `LICENSE` file.
