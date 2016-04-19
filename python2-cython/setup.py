import os
import sys
from distutils.core import setup, Extension
from Cython.Build import cythonize

compile_args = ['-g', '-std=c++11', '-stdlib=libc++']

if sys.platform == 'darwin':
    # Miniconda 3.19 provided Python on OSX is built against OSX deployment target version 10.5
    # this doesn't work with C++11 in libc++. Compiling without the following directive
    # then gives a clang: error:
    # invalid deployment target for -stdlib=libc++ (requires OS X 10.7 or later)
    compile_args.append('-mmacosx-version-min=10.7')

basics_module = Extension('challenge.basics',
    sources=[
        os.path.join('challenge', 'basics.pyx'),
        os.path.join('..', 'src', 'basics.cpp')
    ],
    include_dirs=[
        os.path.join('..', 'include')
    ],
    extra_compile_args=compile_args,
    language='c++')

containers_module = Extension('challenge.containers',
    sources=[
        os.path.join('challenge', 'containers.pyx'),
        os.path.join('..', 'src', 'containers.cpp')
    ],
    include_dirs=[
        os.path.join('..', 'include')
    ],
    extra_compile_args=compile_args,
    language='c++')

converters_module = Extension(
    'challenge.converters',
    sources=[
        os.path.join('challenge', 'converters.i'),
    ],
    include_dirs=[
        os.path.join('..', 'include'),
        os.path.join('include')
    ],
    swig_opts = ['-modern', '-c++', '-Iinclude', '-noproxy'],
    extra_compile_args=compile_args,
)

setup(
    name='challenge',
    packages=['challenge'],
    version='1.0',
    description='C++/Python bindings challenge with Cython',
    ext_modules=cythonize(basics_module) + cythonize(containers_module) + [converters_module, ],
)
