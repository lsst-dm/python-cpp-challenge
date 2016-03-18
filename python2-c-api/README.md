# Hand-written bindings with the Python 2 C API

This directory includes a challenge solution that uses hand-written wrappers that utilize just the Python C API, as well as a build system utilizing the ubiquitous setuptools module.  To try it out, just do:

python setup.py build
python setup.py test

Note that you'll need to have both setuptools and Swig installed to run all the tests.

This solution to the challenge is currently incomplete, and what's present has a few flaws that aren't apparent from the tests:

- Bindings for basics::Doodad::get_secret are not sufficiently careful about memory.  The returned Secret object does not hold a reference to the Doodad that owns it, making it possible for the latter to be destroyed first, causing the Secret to dangle.  This could be fixed by defining a new Python type for Secret instead of using Capsules, which is just a bit more work (and demonstrating Capsule is also useful, even if it isn't a perfect fit).

- The C API provided for downstream use by Swig typemaps requires setting RTLD flags, which is messy and not completely portable.  AFAIK, it is also required for any use of Swig, but we could have written a C API using the macro-heavy approach Capsule approach advocated in the Python docs, which would have made downstream use by something other than Swig not rely on that approach to linking.

This approach also obviously requires more code than most people would want to write for anything but the simplest C++ libraries.  It produces a polished, complete Python interface, and there's little guesswork as the Python C API is very well documented, but there are very clear tradeoffs in readibility and sheer quantity of code for the bindings developer.
