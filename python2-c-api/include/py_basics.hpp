#ifndef CHALLENGE_py_basics_hpp_INCLUDED
#define CHALLENGE_py_basics_hpp_INCLUDED

#include "Python.h"
#include "basics.hpp"

template <typename T> class Py;

template <>
class Py<basics::WhatsIt> {
public:

    static PyObject * to_python(basics::WhatsIt const & it);

    static bool from_python(PyObject * p, basics::WhatsIt * it);

};

template <>
class Py<basics::Secret> {
public:

    static PyObject * to_python(basics::Secret const * s);

    static bool cptr_from_python(PyObject * p, basics::Secret const ** s);

};

template <>
class Py<basics::Doodad> {
public:

    static PyTypeObject * get_type();

    static bool check(PyObject * p);

    static bool is_frozen(PyObject * p);

    static PyObject * to_python(std::shared_ptr<basics::Doodad> s);

    static PyObject * to_python(std::shared_ptr<basics::Doodad const> s);

    static bool sptr_from_python(
        PyObject * p,
        std::shared_ptr<basics::Doodad> * s
    );

    static bool csptr_from_python(
        PyObject * p,
        std::shared_ptr<basics::Doodad const> * s
    );

};

#endif // !CHALLENGE_py_basics_hpp_INCLUDED
