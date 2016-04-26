import sys

# This sequence will get a lot cleaner for Python 3, for which the necessary
# flags should all be in the os module.
import ctypes
flags = ctypes.RTLD_GLOBAL
try:
    import DLFCN
    flags |= DLFCN.RTLD_NOW
except ImportError:
    flags |= 0x2  # works for Linux and Mac, only platforms I care about now.
sys.setdlopenflags(flags)

# Ensure basics modules is loaded first, since we need its
# symbols for anything else.
from . import basics
