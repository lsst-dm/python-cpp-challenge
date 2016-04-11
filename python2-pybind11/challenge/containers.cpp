#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "basics.hpp"
#include "containers.hpp"

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

namespace containers {

PYBIND11_PLUGIN(containers) {
    py::module m("containers", "wrapped C++ containers module");

    py::class_<containers::DoodadSet>(m, "DoodadSet")
        .def(py::init<>())
        .def("__len__", &containers::DoodadSet::size)
        .def("add", [](containers::DoodadSet &ds, std::shared_ptr<basics::Doodad> &d) { ds.add(d); })
        .def("add", [](containers::DoodadSet &ds, std::pair<std::string, int> p) { ds.add(basics::WhatsIt{p.first, p.second}) ; })
        .def("as_dict", &containers::DoodadSet::as_map)
        .def("as_list", &containers::DoodadSet::as_vector)
        .def("assign", &containers::DoodadSet::assign);

    return m.ptr();
}

} // namespace containers

