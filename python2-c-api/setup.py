import os
from setuptools import setup, Extension

basics_module = Extension(
    'challenge.basics',
    sources=[
        os.path.join('challenge', 'py_basics.cpp'),
        os.path.join('..', 'src', 'basics.cpp')
    ],
    include_dirs=[
        os.path.join('..', 'include'),
        os.path.join('include')
    ],
    extra_compile_args=['-std=c++11']
)

converters_module = Extension(
    'challenge.converters',
    sources=[
        os.path.join('challenge', 'converters.i')
    ],
    include_dirs=[
        os.path.join('..', 'include'),
        os.path.join('include')
    ],
    swig_opts = ["-modern", "-c++", "-Iinclude", "-noproxy"],
    extra_compile_args=['-std=c++11']
)

setup(
    name='challenge',
    packages=['challenge'],
    version='1.0',
    test_suite = 'tests',
    description='C++/Python bindings challenge with raw Python C API',
    ext_modules=[basics_module, converters_module],
    #py_modules=["challenge.converters"],
)
