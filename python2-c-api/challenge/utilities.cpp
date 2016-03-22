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

struct WrappedIterator {
    PyObject_HEAD
    PyObject * owner;
    std::unique_ptr<IteratorHolder> holder;
};

static void WrappedIterator_destroy(WrappedIterator * self) {
    Py_XDECREF(self->owner);
    typedef std::unique_ptr<IteratorHolder> Holder;
    self->holder.~Holder();
    self->ob_type->tp_free((PyObject*)self);
}

static PyObject * WrappedIterator_iter(PyObject * self) {
    Py_INCREF(self);
    return self;
}

static PyObject * WrappedIterator_next(WrappedIterator * self) {
    return self->holder->next();
}

static PyTypeObject WrappedIterator_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                           /*ob_size*/
    "challenge.basics.Iterator", /*tp_name*/
    sizeof(WrappedIterator),     /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)WrappedIterator_destroy, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,        /*tp_flags*/
    "Wrapper for C++ iterators",  /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    (getiterfunc)WrappedIterator_iter,  /* tp_iter */
    (iternextfunc)WrappedIterator_next,  /* tp_iternext */
    0,                         /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    0,                         /* tp_init */
    0,                         /* tp_alloc */
    0,                         /* tp_new */
};

PyObject * wrapIterator(
    PyObject * owner,
    std::unique_ptr<IteratorHolder> holder
) {
    WrappedIterator * self =
        (WrappedIterator*)WrappedIterator_Type.tp_alloc(
            &WrappedIterator_Type, 0
        );
    self->owner = owner;
    new (&self->holder) std::unique_ptr<IteratorHolder>(std::move(holder));
    return (PyObject*)self;
}

} // utilities

// ----------------------------------------------------------------------------
// Module Bindings
// ----------------------------------------------------------------------------

static PyMethodDef methods[] = {
    {nullptr}
};

extern "C" {

#ifndef PyMODINIT_FUNC  /* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initutilities(void) {
    if (PyType_Ready(&utilities::WrappedIterator_Type) < 0) return;

    PyObject* m = Py_InitModule3(
        "utilities", methods,
        "utilities for wrapping C++ classes"
    );

    if (!m) return;

    Py_INCREF(&utilities::WrappedIterator_Type);
    PyModule_AddObject(m, "WrappedIterator",
                       (PyObject*)&utilities::WrappedIterator_Type);
}

} // extern "C"
