#include <pybind11/pybind11.h>

#include "basics.hpp"

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

namespace basics {

PYBIND11_PLUGIN(basics) {
    py::module m("basics", "wrapped C++ basics module");

    m.def("compare", &compare);
    m.def("adjacent", &adjacent);

    py::class_<Secret>(m, "Secret");

    py::class_<Doodad, std::shared_ptr<Doodad>>(m, "Doodad")
        .def(py::init<const std::string &, int>(), py::arg("name"), py::arg("value") = 1)
        .def("__init__",
            [](Doodad &instance, std::pair<std::string, int> p) {
                new (&instance) Doodad(WhatsIt{p.first, p.second});
            }
        )
        .def_readwrite("name", &Doodad::name)
        .def_readwrite("value", &Doodad::value)
        .def_static("get_const", &Doodad::get_const) // does not preserve const :(

        // no mapping for unique_ptr but shared_ptr<Doodad> maps to Python Doodad directly
        .def("clone", [](const Doodad &d) { return std::shared_ptr<Doodad>(d.clone().release()); })
        .def("get_secret", &Doodad::get_secret, py::return_value_policy::reference_internal)
        .def("write", [](const Doodad &d) { auto tmp = d.write(); return make_pair(tmp.a, tmp.b); })
        .def("read", [](Doodad &d, std::pair<std::string, int> p) { d.read(WhatsIt{p.first, p.second}) ; });
    return m.ptr();
}

} // namespace basics

