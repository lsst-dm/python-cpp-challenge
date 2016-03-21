#ifndef CHALLENGE_py_extensions_hpp_INCLUDED
#define CHALLENGE_py_extensions_hpp_INCLUDED

#include "py_basics.hpp"
#include "extensions.hpp"

namespace extensions {

class PyThingamajig : public basics::PyDoodad {
public:

    // Python type object used to wrap the base Thingamajig.
    static PyTypeObject * get_type();

    // Return true if the given Python object holds a Thingamajig.
    static bool check(PyObject * p);

    // Conversion function for use with PyArg_* functions.
    // Read a shared_ptr<Thingamajig> into the given variable, if the given
    // Python object holds one.  Return true on success, false (with an
    // exception raised) on failure.
    template <typename T>
    static bool sptr_from_python(
        PyObject * p,
        std::shared_ptr<Thingamajig<T>> * s
    );

    // Read a shared_ptr<Thingamajig<T> const> into the given variable, if the
    // given Python object holds one.  Return true on success, false (with an
    // exception raised) on failure.
    template <typename T>
    static bool csptr_from_python(
        PyObject * p,
        std::shared_ptr<Thingamajig<T> const> * s
    );

};

} // namespace extensions

#endif // !CHALLENGE_py_extensions_hpp_INCLUDED
