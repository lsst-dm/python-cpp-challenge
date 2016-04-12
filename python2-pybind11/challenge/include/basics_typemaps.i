%{
#include "basics.hpp"
#include <pybind11/pybind11.h>
%}

%typemap(out) std::shared_ptr<basics::Doodad> {
    /* Use a pybind11 typecaster to create a PyObject from a shared_ptr<Doodad> */
    pybind11::detail::type_caster<std::shared_ptr<basics::Doodad>> caster;
    pybind11::handle out = caster.cast($1, pybind11::return_value_policy::take_ownership, pybind11::handle());
    $result = out.ptr();
}

//%typemap(out) std::shared_ptr<basics::Doodad const> {
//    $result = basics::PyDoodad::to_python($1);
//}
//
//%typemap(in) basics::Doodad const &
//    (std::shared_ptr<basics::Doodad const> tmp)
//{
//    if (!basics::PyDoodad::csptr_from_python($input, &tmp)) {
//        return nullptr;
//    }
//    // const_cast below shouldn't be necessary; it's a Swig weirdness.
//    $1 = const_cast<basics::Doodad*>(tmp.get());
//}
//
//%typemap(in) basics::Doodad & (std::shared_ptr<basics::Doodad> tmp) {
//    if (!basics::PyDoodad::sptr_from_python($input, &tmp)) {
//        return nullptr;
//    }
//    $1 = tmp.get();
//}

%typemap(in) std::shared_ptr<basics::Doodad> {
    /* First make a pybind11 object handler around the PyObject *
     * Then, cast it to a shared_ptr<Doodad> using the pybind11 caster. */
    pybind11::object p{$input, true};
    try {
        std::shared_ptr<basics::Doodad> ptr(p.cast<std::shared_ptr<basics::Doodad>>());
        $1 = ptr;
    } catch(...) {
        return nullptr;
    }
}

//%typemap(in) std::shared_ptr<basics::Doodad const> {
//    std::shared_ptr<basics::Doodad const> tmp;
//    if (!basics::PyDoodad::csptr_from_python($input, &tmp)) {
//        return nullptr;
//    }
//    $1 = tmp;
//}
