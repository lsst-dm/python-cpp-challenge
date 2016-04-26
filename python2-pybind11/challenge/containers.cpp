#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "basics.hpp"
#include "containers.hpp"

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

PYBIND11_PLUGIN(containers) {
    py::module m("containers", "wrapped C++ containers module");

    py::class_<containers::DoodadSet> c(m, "DoodadSet");

    c.def(py::init<>())
        .def("__len__", &containers::DoodadSet::size)
        .def("add", (void (containers::DoodadSet::*)(std::shared_ptr<basics::Doodad>)) &containers::DoodadSet::add)
        .def("add", [](containers::DoodadSet &ds, std::pair<std::string, int> p) { ds.add(basics::WhatsIt{p.first, p.second}) ; })
        .def("__iter__", [](containers::DoodadSet &ds) { return py::make_iterator(ds.begin(), ds.end()); },
             py::keep_alive<0, 1>())
        .def("as_dict", &containers::DoodadSet::as_map)
        .def("as_list", &containers::DoodadSet::as_vector)
        .def("assign", &containers::DoodadSet::assign);

    c.attr("Item") = py::module::import("challenge.basics").attr("Doodad");

    return m.ptr();
}

