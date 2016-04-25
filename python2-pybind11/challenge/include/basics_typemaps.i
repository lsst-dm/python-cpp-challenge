%{
#include "basics.hpp"
#include <pybind11/pybind11.h>
%}

%typemap(out) std::shared_ptr<basics::Doodad> {
    try {
        pybind::object out = pybind11:cast($1);
        $result = out.ptr();
    } catch (...) {
        PyErr_SetString(PyExc_RuntimeError, "cast error");
        return nullptr;
    }
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
    try {
        $1 = pybind11::handle($input).cast<std::shared_ptr<basics::Doodad>>();
    } catch(...) {
        PyErr_SetString(PyExc_RuntimeError, "cast error");
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
