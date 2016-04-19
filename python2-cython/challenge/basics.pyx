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
    cdef _WhatsIt w = _WhatsIt(t[0], t[1])

    return w

cdef class Secret:
    cdef const _Secret *thisptr

cdef class Doodad:
    def __init__(self, name=None, value=1, init=True):
        if init:
            if isinstance(name, tuple):
                self.thisptr.reset(new _Doodad(from_tuple(name)))
            elif isinstance(name, str):
                self.thisptr.reset(new _Doodad(name, value))
            else:
                raise TypeError("a string or tuple is required")

    def __richcmp__(self, other, int op):
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
        d = Doodad(init=False)

        d.thisptr = move(deref(self.thisptr).clone())

        return d

    @staticmethod
    def get_const():
        d = ImmutableDoodad(init=False)
        d.constptr = _Doodad.get_const()

        return d

    def read(self, t):
        deref(self.thisptr).read(from_tuple(t))

    def write(self):
        cdef _WhatsIt w = deref(self.thisptr).write()

        return (w.a, w.b)

    def get_secret(self):
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
        d = ImmutableDoodad(init=False)
        d.constptr = _Doodad.get_const()

        return d

    def write(self):
        cdef _WhatsIt w = deref(self.constptr).write()

        return (w.a, w.b)

    property name:
        def __get__(self):
            return deref(self.constptr).name

    property value:
        def __get__(self):
            return deref(self.constptr).value

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

