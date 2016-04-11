#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "basics.hpp"
#include "containers.hpp"

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);

namespace containers {

class DoodadSetIterator {
public:
    /* Keep copy of DoodadSet elements (as shared_ptrs)
     * This is needed because the iterator might outlive the container.
     * A more elegant solution would be to have DoodadSet inherit from
     * std::enable_shared_from_this<DoodadSet>, so that an
     * iterator can keep the container alive while it is still in use
     * even though the container has been garbage collected on the Python
     * side, but that would require modifying the C++ implementation. */
    DoodadSetIterator(std::vector<std::shared_ptr<basics::Doodad>> &&seq_) : seq{std::move(seq_)} {
        it = begin(seq);
        it_end = end(seq);
    };

    std::shared_ptr<basics::Doodad> next() {
        if (it == it_end) {
            throw py::stop_iteration();
        } else {
            return *it++;
        }
    };

private:
    std::vector<std::shared_ptr<basics::Doodad>> seq;
    std::vector<std::shared_ptr<basics::Doodad>>::const_iterator it;
    std::vector<std::shared_ptr<basics::Doodad>>::const_iterator it_end;
};

PYBIND11_PLUGIN(containers) {
    py::module m("containers", "wrapped C++ containers module");

    py::class_<containers::DoodadSet>(m, "DoodadSet")
        .def(py::init<>())
        .def("__len__", &containers::DoodadSet::size)
        .def("add", [](containers::DoodadSet &ds, std::shared_ptr<basics::Doodad> &d) { ds.add(d); })
        .def("add", [](containers::DoodadSet &ds, std::pair<std::string, int> p) { ds.add(basics::WhatsIt{p.first, p.second}) ; })
        .def("__iter__", [](containers::DoodadSet &ds) { return DoodadSetIterator{ds.as_vector()}; })
        .def("as_dict", &containers::DoodadSet::as_map)
        .def("as_list", &containers::DoodadSet::as_vector)
        .def("assign", &containers::DoodadSet::assign);

    py::class_<containers::DoodadSetIterator>(m, "DoodadSetIterator")
        .def("__iter__", [](DoodadSetIterator &it) -> DoodadSetIterator& { return it; })
        .def("__next__", &DoodadSetIterator::next);

    return m.ptr();
}

} // namespace containers

