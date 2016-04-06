import os, sys

from distutils.core import setup, Extension
from distutils import sysconfig

cpp_args = ['-std=c++14', '-stdlib=libc++', '-mmacosx-version-min=10.7']

if sys.platform == 'darwin':
    vars = sysconfig.get_config_vars()
    vars['LDSHARED'] = vars['LDSHARED'].replace('-bundle', '-dynamiclib')

ext_modules = [
    Extension(
	'libbasics',
        ['src/basics.cpp'],
        include_dirs=['include'],
	language='c++',
	extra_compile_args = cpp_args,
    ),
    Extension(
	'libcontainers',
        ['src/containers.cpp'],
        include_dirs=['include'],
	language='c++',
	extra_compile_args = cpp_args,
    ),
    Extension(
        'challenge.basics',
        ['challenge/basics.cpp'],
        include_dirs=['pybind11/include', 'include'],
        language='c++',
        library_dirs=['.'],
        libraries=['basics'],
	extra_compile_args = cpp_args,
#	extra_link_args = ['-v'],
    ),
    Extension(
        'challenge.containers',
        ['challenge/containers.cpp'],
        include_dirs=['pybind11/include', 'include'],
        language='c++',
        library_dirs=['.'],
        libraries=['basics','containers'],
	extra_compile_args = cpp_args,
#	extra_link_args = ['-v'],
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
