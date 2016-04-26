import os, sys
from pip import locations
from setuptools import setup, Extension
from distutils import sysconfig

# Remove the "-Wstrict-prototypes" compiler option, which isn't valid for C++.
import distutils.sysconfig
cfg_vars = distutils.sysconfig.get_config_vars()
for key, value in cfg_vars.items():
    if type(value) == str:
        cfg_vars[key] = value.replace("-Wstrict-prototypes", "")

kwds = dict(
    extra_compile_args=['-std=c++11'],
    include_dirs=[
        os.path.join('..', 'include'),
        os.path.join('include'),
        os.path.dirname(locations.distutils_scheme('pybind11')['headers'])
    ],
)

if sys.platform == 'darwin':
    # Miniconda 3.19 provided Python on OSX is built against OSX deployment target version 10.5
    # this doesn't work with C++11 in libc++. Compiling without the following directive
    # then gives a clang: error:
    # invalid deployment target for -stdlib=libc++ (requires OS X 10.7 or later)
    kwds["extra_compile_args"].append('-mmacosx-version-min=10.7')
    kwds["extra_compile_args"].append('-stdlib=libc++')


ext_modules = [
    Extension(
        'challenge.basics',
        sources=[
            os.path.join('challenge', 'basics.cpp'),
            os.path.join('..', 'src', 'basics.cpp')
        ],
        **kwds
    ),
    Extension(
        'challenge.containers',
        sources=[
            os.path.join('challenge', 'containers.cpp'),
            os.path.join('..', 'src', 'containers.cpp')
        ],
        **kwds
    ),
    Extension(
        'challenge.converters', ['challenge/converters.i'],
        swig_opts=["-modern", "-c++", "-Ichallenge/include", "-noproxy"],
        **kwds
    ),
]

setup(
    name='challenge',
    version='0.0.1',
    author='Pim Schellart',
    author_email='P.Schellart@princeton.edu',
    description='Solution to the Python C++ bindings challenge with pybind11.',
    ext_modules=ext_modules,
)
