"""Definitions of Python extension types for basics module
"""
from libcpp cimport bool
from libcpp.memory cimport shared_ptr
from cython.operator cimport dereference as deref
from cpython.object cimport Py_EQ, Py_NE

from _basics cimport move
from _basics cimport compare as _compare
from _basics cimport adjacent as _adjacent
from basics cimport Doodad
from _basics cimport Doodad as _Doodad
from _basics cimport Secret as _Secret
from _basics cimport WhatsIt as _WhatsIt


cpdef compare(Secret a, Secret b):
    return _compare(deref(a.thisptr), deref(b.thisptr))


cpdef adjacent(Secret a, Secret b):
    return _adjacent(deref(a.thisptr), deref(b.thisptr))


cdef _WhatsIt from_tuple(tuple t):
    """Helper function only visible from Cython.
    """
    cdef _WhatsIt w = _WhatsIt(t[0], t[1])
    return w


cdef class Secret:
    """Opaque type only for use by Doodad.
    """
    cdef const _Secret *thisptr


cdef class Doodad:
    """Python interface to C++ type Doodad.

    Parameters
    ----------
    name : str | tuple
        When tuple, a pair of name, value. Otherwise a string.
    value : int
        A value
    """
    def __init__(self, name=None, value=1, init=True):

        if init:
            if isinstance(name, tuple):
                self.thisptr.reset(new _Doodad(from_tuple(name)))
            elif isinstance(name, str):
                self.thisptr.reset(new _Doodad(name, value))
            else:
                raise TypeError("a string or tuple is required")

    def __richcmp__(self, other, int op):
        """Comparison operator
        
        provides '==' and '!='
        """
        if op == Py_EQ and isinstance(other, Doodad):
            return isEqualDD(self, other)
        elif op == Py_EQ and isinstance(other, ImmutableDoodad):
            return isEqualDI(self, other)
        elif op == Py_NE and isinstance(other, Doodad):
            return isNotEqualDD(self, other)
        elif op == Py_NE and isinstance(other, ImmutableDoodad):
            return isNotEqualDI(self, other)
        else:
            raise NotImplementedError


    def clone(self):
        """Calls C++ clone method and returns a new Python Doodad
        """
        d = Doodad(init=False)
        d.thisptr = move(deref(self.thisptr).clone())
        return d

    @staticmethod
    def get_const():
        """Returns an ImmutableDoodad instance
        """
        d = ImmutableDoodad(init=False)
        d.constptr = _Doodad.get_const()
        return d

    def read(self, t):
        """Read new data

        Parameters
        ----------
        t : tuple
            (name, value) pair of (str, int) type.
        """
        deref(self.thisptr).read(from_tuple(t))

    def write(self):
        """Write current data

        Returns tuple with (name, value)
        """
        cdef _WhatsIt w = deref(self.thisptr).write()
        return (w.a, w.b)

    def get_secret(self):
        """Get opaque Secret object
        """
        s = Secret()
        s.thisptr = &deref(self.thisptr).get_secret()
        return s

    property name:
        def __get__(self):
            return deref(self.thisptr).name
        def __set__(self, _name):
            deref(self.thisptr).name = _name

    property value:
        def __get__(self):
            return deref(self.thisptr).value
        def __set__(self, _value):
            deref(self.thisptr).value = _value


cdef class ImmutableDoodad:
    """Python interface to C++ type 'const Doodad'.

    Parameters
    ----------
    name : str | tuple
        When tuple, a pair of name, value. Otherwise a string.
    value : int
        A value
    """
    cdef shared_ptr[const _Doodad] constptr

    def __init__(self, name=None, value=1, init=True):

        if init:
            if isinstance(name, tuple):
                self.constptr.reset(new _Doodad(from_tuple(name)))
            elif isinstance(name, str):
                self.constptr.reset(new _Doodad(name, value))
            else:
                raise TypeError("a string or tuple is required")

    def __richcmp__(self, other, int op):
        """Comparison operator
        
        provides '==' and '!='
        """
        if op == Py_EQ and isinstance(other, ImmutableDoodad):
            return isEqualII(self, other)
        elif op == Py_EQ and isinstance(other, Doodad):
            return isEqualID(self, other)
        elif op == Py_NE and isinstance(other, ImmutableDoodad):
            return isNotEqualII(self, other)
        elif op == Py_NE and isinstance(other, Doodad):
            return isNotEqualID(self, other)
        else:
            raise NotImplementedError

    @staticmethod
    def get_const():
        """Returns an ImmutableDoodad instance
        """
        d = ImmutableDoodad(init=False)
        d.constptr = _Doodad.get_const()
        return d

    def write(self):
        """Write current data

        Returns tuple with (name, value)
        """
        cdef _WhatsIt w = deref(self.constptr).write()
        return (w.a, w.b)

    property name:
        def __get__(self):
            return deref(self.constptr).name

    property value:
        def __get__(self):
            return deref(self.constptr).value


# Helper functions for comparison operator
cdef isEqualDD(Doodad a, Doodad b):
    return a.thisptr.get() == b.thisptr.get()


cdef isNotEqualDD(Doodad a, Doodad b):
    return a.thisptr.get() != b.thisptr.get()


cdef isEqualII(ImmutableDoodad a, ImmutableDoodad b):
    return a.constptr.get() == b.constptr.get()


cdef isNotEqualII(ImmutableDoodad a, ImmutableDoodad b):
    return a.constptr.get() != b.constptr.get()


cdef isEqualDI(Doodad a, ImmutableDoodad b):
    return a.thisptr.get() == b.constptr.get()


cdef isNotEqualDI(Doodad a, ImmutableDoodad b):
    return a.thisptr.get() != b.constptr.get()


cdef isEqualID(ImmutableDoodad a, Doodad b):
    return a.constptr.get() == b.thisptr.get()


cdef isNotEqualID(ImmutableDoodad a, Doodad b):
    return a.constptr.get() != b.thisptr.get()


cdef public newDoodadFromSptr(shared_ptr[_Doodad] _d):
    """Create new Doodad from shared_ptr<Doodad>
    """
    d = Doodad(init=False)
    d.thisptr = move(_d)
    return d


cdef public newImmutableDoodadFromCsptr(shared_ptr[const _Doodad] _d):
    """Create new ImmutableDoodad from shared_ptr<const Doodad>
    """
    d = ImmutableDoodad(init=False)
    d.constptr = _d # should really be move, but cython doesn't like this
    return d


# Cast might fail so marked with except +
cdef public bool sptrFromDoodad(object _d, shared_ptr[_Doodad] *ptr) except + :
    """Get shared_ptr<Doodad> from input Python object if it is a Doodad
    """
    d = <Doodad?> _d
    ptr[0] = d.thisptr
    return True # cannot catch exception here


# Cast might fail so marked with except +
cdef public bool csptrFromImmutableDoodad(object _d, shared_ptr[const _Doodad] *ptr) except + :
    """Get shared_ptr<const Doodad> from input Python object if it is an ImmutableDoodad
    """
    d = <ImmutableDoodad?> _d
    ptr[0] = d.constptr
    return True # cannot catch exception here

