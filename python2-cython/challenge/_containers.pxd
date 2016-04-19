from libcpp.memory cimport unique_ptr
from libcpp.memory cimport shared_ptr
from libcpp.string cimport string
from libcpp.vector cimport vector
from libcpp.map cimport map
from libcpp cimport bool
from _basics cimport Doodad
from _basics cimport WhatsIt

cdef extern from "containers.hpp" namespace "containers":
    cdef cppclass DoodadSet:
        size_t size() const
        vector[shared_ptr[Doodad]].const_iterator begin() const
        vector[shared_ptr[Doodad]].const_iterator end() const
        vector[shared_ptr[Doodad]] as_vector() const
        void assign(vector[shared_ptr[Doodad]] &items)
        void add(shared_ptr[Doodad] item)
        map[string, shared_ptr[Doodad]] as_map() const;
#        void add(WhatsIt &it)

