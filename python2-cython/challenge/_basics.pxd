"""Declarations of C++ types for basics module to make them visible to Cython
"""

from libcpp.memory cimport unique_ptr
from libcpp.memory cimport shared_ptr
from libcpp.string cimport string
from libcpp cimport bool

cdef extern from "basics.hpp" namespace "basics":
    struct WhatsIt:
        string a
        int b

    cdef cppclass Secret:
        pass

    cdef cppclass Doodad:
        Doodad(string, int)
        Doodad(WhatsIt)
        void read(WhatsIt)
        WhatsIt write()
        unique_ptr[Doodad] clone()
        @staticmethod
        shared_ptr[const Doodad] get_const()

        const Secret& get_secret()

        string name
        int value

    cdef bool compare(Secret &a, Secret &b)
    cdef bool adjacent(Secret &a, Secret &b)

cdef extern from "<utility>" namespace "std" nogil:
# having two different declarations with the same argument
# type but different return type is not allowed by Cython
# fortunately not needed for this test...
#    cdef unique_ptr[Doodad] move(unique_ptr[Doodad])
    cdef shared_ptr[Doodad] move(unique_ptr[Doodad])
    cdef shared_ptr[Doodad] move(shared_ptr[Doodad])

