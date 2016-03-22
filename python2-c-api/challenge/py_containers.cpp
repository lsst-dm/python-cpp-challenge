#include "py_containers.hpp"

using basics::Doodad;
using basics::PyDoodad;
using utilities::Py;

namespace utilities {

template class Py<containers::DoodadSet>;

} // namespace utilities

namespace containers {

static Py<DoodadSet> * PyDoodadSet_new(
    PyTypeObject * type, PyObject *, PyObject *
) {
    return Py<DoodadSet>::create(type);
}

static int PyDoodadSet_init(
    Py<DoodadSet> * self, PyObject * args, PyObject * kwds
) {
    static char const * kwd_names[] = {"other", nullptr};
    std::shared_ptr<DoodadSet const> other;
    if (
        PyArg_ParseTupleAndKeywords(
            args, kwds, "|O&", const_cast<char**>(kwd_names),
            &PyDoodadSet::csptr_from_python, &other
        )
    ) {
        if (other) {
            self->instance = std::shared_ptr<DoodadSet>(new DoodadSet(*other));
        } else if (PyErr_Occurred()) {
            return -1;
        } else {
            self->instance = std::shared_ptr<DoodadSet>(new DoodadSet());
        }
        return 0;
    }
    return -1;
}

Py_ssize_t PyDoodadSet_Size(Py<DoodadSet> * self) {
    if (!self->instance) {
        PyErr_SetString(PyExc_TypeError, "Uninitialized DoodadSet");
        return -1;
    }
    return self->instance->size();
}

static PySequenceMethods PyDoodadSet_sequence = {
    (lenfunc)PyDoodadSet_Size
};

static PyMappingMethods PyDoodadSet_mapping = {
    (lenfunc)PyDoodadSet_Size
};

PyObject * PyDoodadSet_as_list(Py<DoodadSet> * self, PyObject *) {
    if (!self->instance) {
        PyErr_SetString(PyExc_TypeError, "Uninitialized DoodadSet");
        return nullptr;
    }
    PyObject * result = PyList_New(self->instance->size());
    if (!result) return nullptr;
    Py_ssize_t index = 0;
    for (
        auto iter = self->instance->begin();
        iter != self->instance->end();
        ++iter, ++index
    ) {
        PyObject * py_item = PyDoodad::to_python(*iter);
        if (py_item) {
            PyList_SET_ITEM(result, index, py_item);
        } else {
            Py_DECREF(result);
            return nullptr;
        }
    }
    return result;
}

PyObject * PyDoodadSet_assign(Py<DoodadSet> * self, PyObject * sequence) {
    if (!self->instance) {
        PyErr_SetString(PyExc_TypeError, "Uninitialized DoodadSet");
        return nullptr;
    }
    std::vector<std::shared_ptr<Doodad>> vector;
    Py_ssize_t len = PyObject_Size(sequence);
    if (len < 0) {
        PyErr_Clear();
    } else {
        vector.reserve(len);
    }
    PyObject * iterator = PyObject_GetIter(sequence);
    if (!iterator) return nullptr;
    while (PyObject * item = PyIter_Next(iterator)) {
        std::shared_ptr<Doodad> d;
        if (PyDoodad::sptr_from_python(item, &d)) {
            vector.push_back(std::move(d));
        } else {
            Py_DECREF(item);
            break;
        }
        Py_DECREF(item);
    }
    Py_DECREF(iterator);
    if (PyErr_Occurred()) {
        return nullptr;
    }
    self->instance->assign(vector);
    Py_RETURN_NONE;
}

PyObject * PyDoodadSet_add(Py<DoodadSet> * self, PyObject * item) {
    if (!self->instance) {
        PyErr_SetString(PyExc_TypeError, "Uninitialized DoodadSet");
        return nullptr;
    }
    std::shared_ptr<Doodad> d;
    if (!PyDoodad::sptr_from_python(item, &d)) {
        PyErr_Clear();
        basics::WhatsIt it;
        if (basics::PyWhatsIt::from_python(item, &it)) {
            d.reset(new Doodad(it));
        }
    }
    self->instance->add(d);
    Py_RETURN_NONE;
}

PyObject * PyDoodadSet_as_dict(Py<DoodadSet> * self, PyObject *) {
    if (!self->instance) {
        PyErr_SetString(PyExc_TypeError, "Uninitialized DoodadSet");
        return nullptr;
    }
    PyObject * result = PyDict_New();
    if (!result) return nullptr;
    for (auto c_item : *self->instance) {
        PyObject * py_item = PyDoodad::to_python(c_item);
        if (py_item) {
            PyObject * key = PyString_FromStringAndSize(
                c_item->name.data(), c_item->name.size()
            );
            if (!key) {
                Py_DECREF(py_item);
                Py_DECREF(result);
                return nullptr;
            }
            PyDict_SetItem(result, key, py_item);
            Py_DECREF(key);
            Py_DECREF(py_item);
        } else {
            Py_DECREF(result);
            return nullptr;
        }
    }
    return result;
}

static PyMethodDef PyDoodadSet_methods[] = {
    {"as_list", (PyCFunction)&PyDoodadSet_as_list, METH_NOARGS,
     "Return a copy of the DoodadSet as a list."},
    {"assign", (PyCFunction)&PyDoodadSet_assign, METH_O,
     "Copy an arbitrary iterable into the DoodadSet."},
    {"add", (PyCFunction)&PyDoodadSet_add, METH_O,
     "Add an item to the DoodadSet."},
    {"as_dict", (PyCFunction)&PyDoodadSet_as_list, METH_NOARGS,
     "Return a copy of the DoodadSet as a {str: Doodad} dict."},
    {nullptr}
};

PyObject * PyDoodadSet_iter(Py<DoodadSet> * self) {
    typedef PyObject * (*Converter)(std::shared_ptr<Doodad>);
    Converter converter = &PyDoodad::to_python;
    return utilities::wrapIterator(
        (PyObject*)self, converter,
        self->instance->begin(), self->instance->end()
    );
}

PyTypeObject * PyDoodadSet::get_type() {
    static PyTypeObject t = {
        PyObject_HEAD_INIT(NULL)
        0,                         /*ob_size*/
        "challenge.containers.DoodadSet", /*tp_name*/
        sizeof(Py<DoodadSet>),          /*tp_basicsize*/
        0,                         /*tp_itemsize*/
        (destructor)Py<DoodadSet>::destroy, /*tp_dealloc*/
        0,                         /*tp_print*/
        0,                         /*tp_getattr*/
        0,                         /*tp_setattr*/
        0,                         /*tp_compare*/
        0,                         /*tp_repr*/
        0,                         /*tp_as_number*/
        &PyDoodadSet_sequence,     /*tp_as_sequence*/
        &PyDoodadSet_mapping,      /*tp_as_mapping*/
        0,                         /*tp_hash */
        0,                         /*tp_call*/
        0,                         /*tp_str*/
        0,                         /*tp_getattro*/
        0,                         /*tp_setattro*/
        0,                         /*tp_as_buffer*/
        Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
        "DoodadSet",               /* tp_doc */
        0,                         /* tp_traverse */
        0,                         /* tp_clear */
        0,                         /* tp_richcompare */
        0,                         /* tp_weaklistoffset */
        (getiterfunc)PyDoodadSet_iter,  /* tp_iter */
        0,                         /* tp_iternext */
        PyDoodadSet_methods,       /* tp_methods */
        0,                         /* tp_members */
        0,                         /* tp_getset */
        0,                         /* tp_base */
        0,                         /* tp_dict */
        0,                         /* tp_descr_get */
        0,                         /* tp_descr_set */
        0,                         /* tp_dictoffset */
        (initproc)PyDoodadSet_init,/* tp_init */
        0,                         /* tp_alloc */
        (newfunc)PyDoodadSet_new,  /* tp_new */
    };
    return &t;
}

bool PyDoodadSet::check(PyObject * p) {
    int r = PyObject_IsSubclass(
        (PyObject*)p->ob_type,
        (PyObject*)PyDoodadSet::get_type()
    );
    if (r < 0) {
        PyErr_Clear();
        return false;
    }
    return r;
}

PyObject * PyDoodadSet::to_python(std::shared_ptr<DoodadSet> s) {
    if (!s) {
        Py_RETURN_NONE;
    }
    Py<DoodadSet> * result = Py<DoodadSet>::create(PyDoodadSet::get_type());
    if (result) {
        result->instance = std::move(s);
    }
    return (PyObject*)result;
}

PyObject * PyDoodadSet::to_python(std::shared_ptr<DoodadSet const> s) {
    if (!s) {
        Py_RETURN_NONE;
    }
    Py<DoodadSet> * result = Py<DoodadSet>::create(PyDoodadSet::get_type());
    if (result) {
        result->instance = std::const_pointer_cast<DoodadSet>(std::move(s));
        result->frozen = true;
    }
    return (PyObject*)result;
}

bool PyDoodadSet::sptr_from_python(
    PyObject * p, std::shared_ptr<DoodadSet> * s
) {
    if (check(p)) {
        Py<DoodadSet> * d = reinterpret_cast<Py<DoodadSet>*>(p);
        *reinterpret_cast<std::shared_ptr<DoodadSet>*>(s) = d->instance;
        if (d->frozen) {
            PyErr_SetString(PyExc_TypeError, "Non-frozen DoodadSet required.");
            return false;
        }
        return true;
    }
    if (p == Py_None) {
        s->reset();
        return true;
    }
    PyErr_SetString(PyExc_TypeError, "Could not convert object to DoodadSet.");
    return false;
}

bool PyDoodadSet::csptr_from_python(
    PyObject * p, std::shared_ptr<DoodadSet const> * s
) {
    if (check(p)) {
        Py<DoodadSet> * d = reinterpret_cast<Py<DoodadSet>*>(p);
        *reinterpret_cast<std::shared_ptr<DoodadSet>*>(s) = d->instance;
        return true;
    }
    if (p == Py_None) {
        s->reset();
        return true;
    }
    PyErr_SetString(PyExc_TypeError, "Could not convert object to DoodadSet.");
    return false;
}

} // namespace containers

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
initcontainers(void) {
    if (PyType_Ready(containers::PyDoodadSet::get_type()) < 0) return;

    PyObject* m = Py_InitModule3(
        "containers", methods,
        "wrappers for classes in containers.hpp"
    );

    if (!m) return;

    Py_INCREF(containers::PyDoodadSet::get_type());
    PyModule_AddObject(
        m, "DoodadSet",
        (PyObject*)containers::PyDoodadSet::get_type()
    );
}

} // extern "C"
