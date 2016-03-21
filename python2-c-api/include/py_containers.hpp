#ifndef CHALLENGE_py_containers_hpp_INCLUDED
#define CHALLENGE_py_containers_hpp_INCLUDED

#include "py_basics.hpp"
#include "containers.hpp"

namespace containers {

struct PyDoodadSet {

    // Python type object used to wrap Doodad.
    static PyTypeObject * get_type();

    // Return true if the given Python object holds a DoodadSet.
    static bool check(PyObject * p);

    // Convert a Doodad shared_ptr to Python.
    static PyObject * to_python(std::shared_ptr<DoodadSet> s);

    // Convert a const Doodad shared_ptr to Python.
    static PyObject * to_python(std::shared_ptr<DoodadSet const> s);

    // Conversion function for use with PyArg_* functions.
    // Read a shared_ptr<Doodad> into the given variable, if the given
    // Python object holds one.  Return true on success, false (with an
    // exception raised) on failure.
    static bool sptr_from_python(
        PyObject * p,
        std::shared_ptr<DoodadSet> * s
    );

    // Read a shared_ptr<Doodad const> into the given variable, if the given
    // Python object holds one.  Return true on success, false (with an
    // exception raised) on failure.
    static bool csptr_from_python(
        PyObject * p,
        std::shared_ptr<DoodadSet const> * s
    );

};

} // namespace containers

#endif // !CHALLENGE_py_containers_hpp_INCLUDED
