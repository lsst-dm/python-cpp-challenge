"""Declarations of Python extension types for basics module
"""
from libcpp.memory cimport shared_ptr
from libcpp.string cimport string

from _basics cimport Doodad as _Doodad


cdef class MutableDoodad:

    cdef shared_ptr[_Doodad] thisptr
