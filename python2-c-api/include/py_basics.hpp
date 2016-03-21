#ifndef CHALLENGE_py_basics_hpp_INCLUDED
#define CHALLENGE_py_basics_hpp_INCLUDED

#include "Python.h"
#include "basics.hpp"

namespace basics {

struct PyWhatsIt {

    // Convert a WhatsIt to its Python representation (a tuple).
    static PyObject * to_python(WhatsIt const & it);

    // Conversion function for use with PyArg_* functions.
    // Read a WhatsIt into the given variable, if the given Python object
    // can be converted to one.  Return true on success, false (with an
    // exception raised) on failure.
    static bool from_python(PyObject * p, WhatsIt * it);

};

struct PySecret {

    // Convert a Secret to its Python representation (a Capsule).
    static PyObject * to_python(Secret const * s);

    // Conversion function for use with PyArg_* functions.
    // Read a Secret* into the given variable, if the given Python object
    // holds one.  Return true on success, false (with an exception raised)
    // on failure.
    static bool cptr_from_python(PyObject * p, Secret const ** s);

};

struct PyDoodad {

    // Python type object used to wrap Doodad.
    static PyTypeObject * get_type();

    // Return true if the given Python object holds a Doodad.
    static bool check(PyObject * p);

    // Convert a Doodad shared_ptr to Python.
    static PyObject * to_python(std::shared_ptr<Doodad> s);

    // Convert a const Doodad shared_ptr to Python.
    static PyObject * to_python(std::shared_ptr<Doodad const> s);

    // Conversion function for use with PyArg_* functions.
    // Read a shared_ptr<Doodad> into the given variable, if the given
    // Python object holds one.  Return true on success, false (with an
    // exception raised) on failure.
    static bool sptr_from_python(
        PyObject * p,
        std::shared_ptr<Doodad> * s
    );

    // Read a shared_ptr<Doodad const> into the given variable, if the given
    // Python object holds one.  Return true on success, false (with an
    // exception raised) on failure.
    static bool csptr_from_python(
        PyObject * p,
        std::shared_ptr<Doodad const> * s
    );

    PyObject_HEAD
    bool frozen;
    std::shared_ptr<Doodad> instance;
};

} // namespace basics

#endif // !CHALLENGE_py_basics_hpp_INCLUDED
