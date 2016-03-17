#ifndef CHALLENGE_py_basics_hpp_INCLUDED
#define CHALLENGE_py_basics_hpp_INCLUDED

#include "Python.h"
#include "basics.hpp"

template <typename T> class Py;

template <>
class Py<basics::WhatsIt> {
public:

    static PyObject * to_python(basics::WhatsIt const & it);

    static bool from_python(PyObject * p, void * it);

};

template <>
class Py<basics::Secret const*> {
public:

    static PyObject * to_python(basics::Secret const * s);

    static bool from_python(PyObject * p, void const ** s);

};

template <>
class Py<std::shared_ptr<basics::Doodad>> {
public:

    static PyObject * to_python(std::shared_ptr<basics::Doodad> s);

    static bool from_python(PyObject * p, void * s);

};

#endif // !CHALLENGE_py_basics_hpp_INCLUDED
