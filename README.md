Crisp
=====

A coding rule validation add-on for **LLVM/Clang**, GPLv3 Licensed.

**Coding Rules** constrain admissible constructs of a language to help
produce better code. Crisp can be used to define and enforce rules for
C and C++, like those in [High Integrity C++] [HICPP], and
[CERT's Secure Coding Standards] [CERT-SCS].

Crisp rules are written in Prolog. A high-level declarative DSL to
easily write new rules is under development. It will be called CRISP,
an acronym for *Coding Rules In Sugared Prolog*.

  [HICPP]:    http://www.codingstandard.com/
  [CERT-SCS]: http://www.cert.org/secure-coding/

Installation
============

Although Crisp should be quite portable, it has been tested on
GNU/Linux only so far.

Prerequisites
-------------

1. Install **LLVM/Clang**. Either you do install from sources or package
   for your distribution, the Clang version shall match with Crisp version.
   Make sure that `llvm-config` and `clang` executables are in the `PATH`
   environment.

2. Install **Lit**. This is optional, do only if you want to run the tests!
   Lit is the [LLVM Integrated Tester][LIT]. If you installed Clang from
   sources, you shall have it automaticaly. If your package manager does
   not provide it, you can simply install from [PyPI][PyPI] because it is
   written in python.

3. Install **SWI-Prolog**. Version 5.10 or greater is recommended. It
   is most likely provided as a pre-compiled package for your
   GNU/Linux distribution. On Debian/Ubuntu (and derivatives) you
   simply need to type `sudo apt-get install swi-prolog`. On other
   systems, you need to be sure that a dynamic library (called
   `libswipl.so` on POSIX systems) is available.

4. Install **Boost C++ Libraries**, version 1.46.1 or greater. Crisp
   relies on header-only libraries. On Debian/Ubuntu (and derivatives)
   you only need to type `sudo apt-get install libboost-dev`.

   [LIT]: http://llvm.org/docs/CommandGuide/lit.html
   [PyPI]: https://pypi.python.org/pypi/lit


Download Crisp source code
--------------------------

Source code is available at GitHub:

    git clone git://github.com/gmarpons/Crisp.git

Build Crisp
-----------

Simply execute the following commands in Crisp source directory:

    mkdir build && cd build
    cmake ..
    make all
    make install  # to install Crisp
    make check    # to run tests
    make package  # to create package

Basic Usage
===========

Current code is still a proof of concept with very limited
functionality. The very small number of coding rules already
implemented can be found in the `prolog/Rules` directory of the source
distribution. You can test them with your own C/C++ code, or using the
example code in `docs/examples`.

For example, you can enable coding rule validation for testing file
`hicpp_3_3_11.cpp` with

    cd $CRISP_SRC_ROOT/docs/examples
    clang++ -cc1                                          \
      -load $CRISP_LIB_PATH/libcrispclang.so              \
      -add-plugin crisp -plugin-arg-crisp SomeHICPPrules  \
      hicpp_3_3_13.cpp

where the meaning of all the options and variables used is the
following:

- `-cc1`: run the Clang compiler, not the driver that invokes the
  different LLVM tools.
- `-load $CRISP_INSTALL_ROOT/lib/libcrispclang.so`: dynamically load a
  plugin that emits a warning message when some coding rule is
  violated. `$CRISP_LIB_PATH` is `/usr/local/lib` if you were not
  defined otherwise at cmake step.
- `-add-plugin crisp-clang`: run the plugin.
- `-plugin-arg-crisp-clang SomeHICPPrules`: an argument to the plugin
  to choose a file with rule definitions (so far codified in
  Prolog). A `.pl` extension is optional. Rule files are first
  searched in the working directory, and then in a specific directory
  of the distribution/installation.
- `hicpp_3_3_13.cpp`: input file with C++ code to compile/analyze.

Three warning messages are expected on *stderr* telling that file
hicpp_3_3_13.cpp contains three violations of rule HICPP 3.3.13.

To run the plugin against your sources you need to tune the build
script of your project. Sure you need to replace the compiler to
Clang. To run the same the check from the previous example, you
need to 'escape' the plugin flags to the compiler, like this:

    CC="clang"
    CXX="clang++"
    CXX_FLAGS+="-Xclang -load -Xclang $CRISP_LIB_PATH/libcrispclang.so "
    CXX_FLAGS+="-Xclang -add-plugin -Xclang crisp "
    CXX_FLAGS+="-Xclang -plugin-arg-crisp -Xclang SomeHICPPrules "

Some rules need a second step (those that rely on alias analysis
information):

    clang++ -cc1                                          \
      -load $CRISP_LIB_PATH/libcrispclang.so              \
      -add-plugin crisp -plugin-arg-crisp SomeHICPPrules  \
      -emit-llvm hicpp_3_4_2.cpp

    opt -analyze                                          \
      -load $CRISP_LIB_PATH/libcrispllvm.so               \
      -crisp-mod -crisp-rules-file SomeHICPPrules         \
      -basicaa hicpp_3_4_2.ll > /dev/null

The first command is the same shown before, but applied to example
file `hicpp_3_4_2.cpp` and `-emit-llvm` to compile to LLVM IR.
The second command runs the optimization and analysis tool of LLVM,
and executes an analysis pass for rule validation based on alias
analysis. The precision of rule checkers depends on the sophistication
of the alias analysis used. `-basicaa` enables a simple alias analysis
algorithm. Other useful alias analysis implementations require
installing loadable modules distributed separately. The command takes
as input a `.ll` file generated in the previous step. The expected
output is a list of two warnings, telling that rule HICPP 3.4.2 has
been violated by two different functions.


Known Issues
============

- Rule violation reporting could be improved, specially for `opt`
  plug-in (assembler names are used to point functions out, and no
  line/column numbers are given to the user).
- Extending the tool with user-defined rules requires a deep knowledge
  of both Prolog and Crisp internals. Next releases will focus on
  providing a high-level rule definition DSL that will be
  automatically translated into Prolog.
