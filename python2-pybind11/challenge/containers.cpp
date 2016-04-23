#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "basics.hpp"
#include "containers.hpp"

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

namespace containers {

class DoodadSetIterator {
public:
    DoodadSetIterator(DoodadSet::iterator b, DoodadSet::iterator e) : it{b}, it_end{e} {};

    std::shared_ptr<basics::Doodad> next() {
        if (it == it_end) {
            throw py::stop_iteration();
        } else {
            return *it++;
        }
    };

private:
    DoodadSet::iterator it;
    DoodadSet::iterator it_end;
};

} // namespace containers

PYBIND11_PLUGIN(containers) {
    py::module m("containers", "wrapped C++ containers module");

    py::class_<containers::DoodadSet>(m, "DoodadSet")
        .def(py::init<>())
        .def("__len__", &containers::DoodadSet::size)
        .def("add", (void (containers::DoodadSet::*)(std::shared_ptr<basics::Doodad>)) &containers::DoodadSet::add)
        .def("add", [](containers::DoodadSet &ds, std::pair<std::string, int> p) { ds.add(basics::WhatsIt{p.first, p.second}) ; })
        .def("__iter__", [](containers::DoodadSet &ds) { return containers::DoodadSetIterator{ds.begin(), ds.end()}; }, py::keep_alive<0,1>())
        .def("as_dict", &containers::DoodadSet::as_map)
        .def("as_list", &containers::DoodadSet::as_vector)
        .def("assign", &containers::DoodadSet::assign);

    py::class_<containers::DoodadSetIterator>(m, "DoodadSetIterator")
        .def("__iter__", [](containers::DoodadSetIterator &it) -> containers::DoodadSetIterator& { return it; })
        .def("__next__", &containers::DoodadSetIterator::next);

    return m.ptr();
}

