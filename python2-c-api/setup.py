import os
from setuptools import setup, Extension

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
        os.path.join('include')
    ],
)

utilities_module = Extension(
    'challenge.utilities',
    sources=[
        os.path.join('challenge', 'utilities.cpp'),
    ],
    **kwds
)

basics_module = Extension(
    'challenge.basics',
    sources=[
        os.path.join('challenge', 'py_basics.cpp'),
        os.path.join('challenge', 'utilities.cpp'),
        os.path.join('..', 'src', 'basics.cpp')
    ],
    **kwds
)

extensions_module = Extension(
    'challenge.extensions',
    sources=[
        os.path.join('challenge', 'py_extensions.cpp'),
        os.path.join('..', 'src', 'extensions.cpp')
    ],
    **kwds
)

containers_module = Extension(
    'challenge.containers',
    sources=[
        os.path.join('challenge', 'py_containers.cpp'),
        os.path.join('..', 'src', 'containers.cpp')
    ],
    **kwds
)

converters_module = Extension(
    'challenge.converters',
    sources=[
        os.path.join('challenge', 'converters.i')
    ],
    swig_opts = ["-modern", "-c++", "-Iinclude", "-noproxy"],
    **kwds
)

setup(
    name='challenge',
    packages=['challenge'],
    version='1.0',
    test_suite = 'tests',
    description='C++/Python bindings challenge with raw Python C API',
    ext_modules=[utilities_module, basics_module, containers_module,
                 extensions_module, converters_module],
)
