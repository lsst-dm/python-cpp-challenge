#include <pybind11/pybind11.h>

#include "basics.hpp"

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

namespace basics {

PYBIND11_PLUGIN(basics) {
    py::module m("basics", "wrapped C++ basics module");

    m.def("compare", &basics::compare);
    m.def("adjacent", &basics::adjacent);

    py::class_<basics::Secret>(m, "Secret");

    py::class_<basics::Doodad, std::shared_ptr<basics::Doodad>>(m, "Doodad")
        .def(py::init<const std::string &, int>(), py::arg("name"), py::arg("value") = 1)
        .def("__init__",
            [](basics::Doodad &instance, std::pair<std::string, int> p) {
                new (&instance) basics::Doodad(basics::WhatsIt{p.first, p.second});
            }
        )
        .def_readwrite("name", &basics::Doodad::name)
        .def_readwrite("value", &basics::Doodad::value)
        .def_static("get_const", &basics::Doodad::get_const) // does not preserve const :(

        // no mapping for unique_ptr but shared_ptr<Doodad> maps to Python Doodad directly
        .def("clone", [](const basics::Doodad &d) { return std::shared_ptr<basics::Doodad>(d.clone().release()); })
        .def("get_secret", &basics::Doodad::get_secret, py::return_value_policy::reference_internal)
        .def("write", [](const basics::Doodad &d) { auto tmp = d.write(); return make_pair(tmp.a, tmp.b); })
        .def("read", [](basics::Doodad &d, std::pair<std::string, int> p) { d.read(basics::WhatsIt{p.first, p.second}) ; });

    return m.ptr();
}

} // namespace basics

