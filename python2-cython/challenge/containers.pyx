"""Definitions of Python extension types for containers module
"""
from libcpp cimport bool
from libcpp.memory cimport shared_ptr
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
from cython.operator cimport dereference as deref
from cython.operator cimport preincrement as incr

from basics import Doodad
from basics cimport Doodad
from _basics cimport move
from _basics cimport Doodad as _Doodad
from _containers cimport DoodadSet as _DoodadSet


cdef class DoodadSet:
    """Python interface to C++ type DoodadSet
    """
    cdef _DoodadSet inst
    cdef vector[shared_ptr[_Doodad]].const_iterator it
    Item = Doodad
    def __len__(self):
        return self.inst.size()

    def __iter__(self):
        # New iterators start at beginning.
        # Note that this is not thread-safe!
        # If thread safety is required represent the
        # iterator as a separate type or return a new object
        self.it = self.inst.begin()
        return self

    def __next__(self):
        if self.it == self.inst.end():
            raise StopIteration()
        d = Doodad(init=False)
        d.thisptr = deref(self.it)
        incr(self.it)
        return d

    cpdef add(self, item) except +:
        """Add Doodad to set

        Parameters
        ----------
        item : Doodad
            The item to add
        """
        if isinstance(item, tuple):
            d = Doodad(item)
        else:
            d = <Doodad?> item
        self.inst.add(d.thisptr)

    cpdef as_list(self):
        """Return Python list of objects in set

        Note that new Python objects will be created share
        the same underlying C++ Doodad's stored in the set.
        """
        cdef vector[shared_ptr[_Doodad]] v = self.inst.as_vector()
        results = []
        for item in v:
            d = Doodad(init=False)
            d.thisptr = move(item)
            results.append(d)
        return results

    cpdef as_dict(self):
        """Return Python dict of objects in set

        Note that new Python objects will be created share
        the same underlying C++ Doodad's stored in the set.
        """
        cdef map[string, shared_ptr[_Doodad]] m = self.inst.as_map()
        results = {}
        for k in m:
            d = Doodad(init=False)
            d.thisptr = move(k.second)
            results[k.first] = d
        return results

    cpdef assign(self, seq) except +:
        """Assign Doodad's to set

        Parameters
        ----------
        seq : sequence
            Any Python sequence (e.g. list, tuple) of Doodad's
        """
        cdef vector[shared_ptr[_Doodad]] v
        for item in seq:
            d = <Doodad?> item
            v.push_back(d.thisptr)
        self.inst.assign(v)
