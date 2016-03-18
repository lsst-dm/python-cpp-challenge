#ifndef CHALLENGE_py_basics_hpp_INCLUDED
#define CHALLENGE_py_basics_hpp_INCLUDED

#include "Python.h"
#include "basics.hpp"

template <typename T> class Py;

template <>
class Py<basics::WhatsIt> {
public:

    // Convert a WhatsIt to its Python representation (a tuple).
    static PyObject * to_python(basics::WhatsIt const & it);

    // Conversion function for use with PyArg_* functions.
    // Read a WhatsIt into the given variable, if the given Python object
    // can be converted to one.  Return true on success, false (with an
    // exception raised) on failure.
    static bool from_python(PyObject * p, basics::WhatsIt * it);

};

template <>
class Py<basics::Secret> {
public:

    // Convert a Secret to its Python representation (a Capsule).
    static PyObject * to_python(basics::Secret const * s);

    // Conversion function for use with PyArg_* functions.
    // Read a Secret* into the given variable, if the given Python object
    // holds one.  Return true on success, false (with an exception raised)
    // on failure.
    static bool cptr_from_python(PyObject * p, basics::Secret const ** s);

};

template <>
class Py<basics::Doodad> {
public:

    // Python type object used to wrap Doodad.
    static PyTypeObject * get_type();

    // Return true if the given Python object holds a Doodad.
    static bool check(PyObject * p);

    // Return true if the given Python Doodad is frozen (i.e. const).
    // Assumes the object is a Doodad.
    static bool is_frozen(PyObject * p);

    // Convert a Doodad shared_ptr to Python.
    static PyObject * to_python(std::shared_ptr<basics::Doodad> s);

    // Convert a const Doodad shared_ptr to Python.
    static PyObject * to_python(std::shared_ptr<basics::Doodad const> s);

    // Conversion function for use with PyArg_* functions.
    // Read a shared_ptr<Doodad> into the given variable, if the given
    // Python object holds one.  Return true on success, false (with an
    // exception raised) on failure.
    static bool sptr_from_python(
        PyObject * p,
        std::shared_ptr<basics::Doodad> * s
    );

    // Read a shared_ptr<Doodad const> into the given variable, if the given
    // Python object holds one.  Return true on success, false (with an
    // exception raised) on failure.
    static bool csptr_from_python(
        PyObject * p,
        std::shared_ptr<basics::Doodad const> * s
    );

};

#endif // !CHALLENGE_py_basics_hpp_INCLUDED
