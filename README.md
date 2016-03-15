# C++/Python Bindings Challenge

This repository contains a small suite of C++ classes designed to highlight any of the most common challenges involved in providing Python bindings to a C++ library, as well as (coming soon) a suite of Python unit tests that attempt to measure the quality of the Python bindings.

The C++ classes themselves are highly artificial, but their interfaces contain represent important and common problems in binding C++ to Python, ranging from trivial (e.g. convert C++ strings to Python str or unicode) to extremely difficult (wrap const references to C++ objects while protecting against dangling pointers and guaranteeing constness).

Submission of both solutions and additional challenges is welcome.  There's no formal competition, but I'm hoping submissions will be enthusiastically discussed and groups (including mine) will find them useful for selecting which binding tools and approaches to consider in the future.

## The Challenge

 - Each of the C++ header/src pairs must be wrapped into a separate Python module within the same package, with both "containers" and "extensions" depending on (i.e. internally importing) "basics", but not depending on each other.

 - Make as many of the unit tests pass as possible, using the smallest amount of *readible*, *class-specific* code; we're trying to use this small package to guess how much code would be required for a much larger suite of libraries with the same conventions, so code that just sets up rules for code generators is in some sense not counted - or at least amortized down.

Some of the tests are *extremely hard*, and many judges will consider a lightweight solution that does not handle some edge case preferable to a heavyweight solution that does, so just use your best judgement.

The formal definition of the desired Python interface is defined by the unit tests, but the C++ header files also contain notes that indicate most of this (as they're a lot easier to read than the tests).

## Submitting a Solution

1. Fork this repository.
2. Create a *branch* for your solution, named after the binding tool and/or any other identifier unique to your approach.
3. Write some bindings.
4. Add a build system: autotools, cmake, scons, and any other standard build tool is welcome, but utilizing just Python distutils is a bonus.
5. Run the tests.
6. Submit your solution as a PR, and summarize your approach and test results there for discussion.

We will not actually be merging solution branches to master, but we hope to collect them within the original repository.  That allows new challenges to be added to master without disrupting existing solutions.

## Submitting a Challenge

1. Fork this repository.
2. Add new C++ code and unit tests as necessary, on the fork's master branch.
3. Update this README if necessary.
4. Submit your challenge as a PR.
