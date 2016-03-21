#include "utilities.hpp"

namespace utilities {

InheritanceTree::InheritanceTree(PyTypeObject * type) :
    _type(type),
    _sibling(nullptr),
    _child(nullptr)
{}

PyTypeObject * InheritanceTree::find(void const * v) const {
    if (_try_cast(v)) {
        if (_child) {
            PyTypeObject * r = _child->find(v);
            if (r) return r;
        }
        return _type;
    }
    if (_sibling) {
        return _sibling->find(v);
    }
    return nullptr;
}

InheritanceTree * InheritanceTree::_add_child(
    std::unique_ptr<InheritanceTree> child
) {
    if (_child) {
        return _child->_add_sibling(std::move(child));
    }
    _child = std::move(child);
    return _child.get();
}

InheritanceTree * InheritanceTree::_add_sibling(
    std::unique_ptr<InheritanceTree> sibling
) {
    if (_sibling) {
        return _sibling->_add_sibling(std::move(sibling));
    }
    _sibling = std::move(sibling);
    return _sibling.get();
}

} // utilities
