%{
#include "basics.hpp"
#include <pybind11/pybind11.h>
%}

%typemap(out) std::shared_ptr<basics::Doodad> {
    // Use a pybind11 typecaster to create a PyObject from a shared_ptr<Doodad>
    pybind11::detail::type_caster<std::shared_ptr<basics::Doodad>> caster;
    pybind11::handle out = caster.cast($1, pybind11::return_value_policy::take_ownership, pybind11::handle());
    $result = out.ptr();
}

%typemap(out) std::shared_ptr<basics::Doodad const> {
    // Use a pybind11 typecaster to create a PyObject from a shared_ptr<Doodad const>
    pybind11::detail::type_caster<std::shared_ptr<basics::Doodad const>> caster;
    pybind11::handle out = caster.cast($1, pybind11::return_value_policy::take_ownership, pybind11::handle());
    $result = out.ptr();
}

%typemap(in) basics::Doodad const & (std::shared_ptr<basics::Doodad const> tmp)
{
    // First make a pybind11 object handler around the PyObject*
    // Then, cast it to a shared_ptr<Doodad> using the pybind11 caster.
    // We declare that shared_ptr above so Swig keeps it alive for the duration
    // of the wrapped function.
    pybind11::object p{$input, true};
    try {
        tmp = p.cast<std::shared_ptr<basics::Doodad const>>();
        $1 = const_cast<basics::Doodad*>(tmp.get());
    } catch(...) {
        PyErr_SetString(PyExc_RuntimeError, "could not cast to Doodad const &");
        return nullptr;
    }
}

%typemap(in) basics::Doodad & (std::shared_ptr<basics::Doodad> tmp) {
    // First make a pybind11 object handler around the PyObject*
    // Then, cast it to a shared_ptr<Doodad> using the pybind11 caster.
    // We declare that shared_ptr above so Swig keeps it alive for the duration
    // of the wrapped function.
    pybind11::object p{$input, true};
    try {
        tmp = p.cast<std::shared_ptr<basics::Doodad>>();
        $1 = tmp.get();
    } catch(...) {
        PyErr_SetString(PyExc_RuntimeError, "could not cast to Doodad &");
        return nullptr;
    }
}

%typemap(in) std::shared_ptr<basics::Doodad> {
    // First make a pybind11 object handler around the PyObject*
    // Then, cast it to a shared_ptr<Doodad> using the pybind11 caster.
    pybind11::object p{$input, true};
    try {
        std::shared_ptr<basics::Doodad> ptr(p.cast<std::shared_ptr<basics::Doodad>>());
        $1 = ptr;
    } catch(...) {
        PyErr_SetString(PyExc_RuntimeError, "could not cast to shared_ptr<Doodad>");
        return nullptr;
    }
}

%typemap(in) std::shared_ptr<basics::Doodad const> {
    // First make a pybind11 object handler around the PyObject*
    // Then, cast it to a shared_ptr<Doodad> using the pybind11 caster.
    pybind11::object p{$input, true};
    try {
        std::shared_ptr<basics::Doodad> ptr(p.cast<std::shared_ptr<basics::Doodad>>());
        $1 = ptr;
    } catch(...) {
        PyErr_SetString(PyExc_RuntimeError, "could not cast to shared_ptr<Doodad>");
        return nullptr;
    }
}
