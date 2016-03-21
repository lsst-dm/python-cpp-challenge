#include "py_extensions.hpp"

using basics::Doodad;
using basics::PyDoodad;

namespace extensions {

// ----------------------------------------------------------------------------
// Thingamajig
// ----------------------------------------------------------------------------

static int PyThingamajig_init(
    PyDoodad * self, PyObject * args, PyObject * kwds
) {
    static char const * kwd_names[] = {
        "extra", "name", "value", "dtype", nullptr
    };
    PyObject * extra = nullptr;
    int name_size = 0;
    int value = 1;
    const char * name_str = nullptr;
    PyObject * dtype = nullptr;
    if (
        PyArg_ParseTupleAndKeywords(
            args, kwds, "Os#|iO", const_cast<char**>(kwd_names),
            &extra, &name_str, &name_size, &value, &dtype
        )
    ) {
        std::string name(name_str, name_size);
        if (!dtype) {
            if (PyObject_IsInstance(extra, (PyObject*)PyDoodad::get_type())) {
                dtype = (PyObject*)PyDoodad::get_type();
            } else if (PyNumber_Check(extra)) {
                dtype = (PyObject*)&PyFloat_Type;
            } else {
                PyErr_SetString(
                    PyExc_TypeError,
                    "Unrecognized type for extra argument to Thingamajig."
                );
                return -1;
            }
        }
        if (dtype == (PyObject*)PyDoodad::get_type()) {
            std::shared_ptr<Doodad> ex;
            if (!PyDoodad::sptr_from_python(extra, &ex)) {
                return -1;
            }
            std::shared_ptr<Doodad> instance(
                new Thingamajig<std::shared_ptr<Doodad>>(ex, name, value)
            );
            self->instance = std::move(instance);
            return 0;
        } else if (dtype == (PyObject*)&PyFloat_Type) {
            double ex = PyFloat_AsDouble(extra);
            if (ex == -1.0 && PyErr_Occurred()) {
                return -1;
            }
            std::shared_ptr<Doodad> instance(
                new Thingamajig<double>(ex, name, value)
            );
            self->instance = std::move(instance);
            return 0;
        } else {
            PyErr_SetString(PyExc_TypeError, "Unrecognized dtype.");
            return -1;
        }
    }
    return -1;
}

static PyObject * PyThingamajig_get_extra(PyDoodad * self, PyObject *) {
    typedef Thingamajig<std::shared_ptr<Doodad>> T1;
    typedef Thingamajig<double> T2;
    if (auto t = std::dynamic_pointer_cast<T1>(self->instance)) {
        return PyDoodad::to_python(t->get_extra());
    }
    if (auto t = std::dynamic_pointer_cast<T2>(self->instance)) {
        return PyFloat_FromDouble(t->get_extra());
    }
    PyErr_SetString(PyExc_SystemError, "Object is not a valid Thingamajig.");
    return nullptr;
}

static PyMethodDef PyThingamajig_methods[] = {
    {"get_extra", (PyCFunction)&PyThingamajig_get_extra, METH_NOARGS,
     "Return the extra object attached to the Thingamajig."},
    {nullptr}
};

static PyObject * PyThingamajig_get_dtype(PyDoodad * self, void *) {
    typedef Thingamajig<std::shared_ptr<Doodad>> T1;
    typedef Thingamajig<double> T2;
    PyObject * result = nullptr;
    if (auto t = std::dynamic_pointer_cast<T1>(self->instance)) {
        result = (PyObject*)PyDoodad::get_type();
    }
    if (auto t = std::dynamic_pointer_cast<T2>(self->instance)) {
        result = (PyObject*)&PyFloat_Type;
    }
    Py_INCREF(result);
    return result;
}

struct PyGetSetDef PyThingamajig_getset[] = {
    {const_cast<char*>("dtype"), (getter)PyThingamajig_get_dtype, nullptr,
     const_cast<char*>("dtype of the Thingamajig"), nullptr},
    {nullptr}
};

PyTypeObject * PyThingamajig::get_type() {
    static PyTypeObject t = {
        PyObject_HEAD_INIT(NULL)
        0,                                    /*ob_size*/
        "challenge.extensions.Thingamajig",   /*tp_name*/
        0,                         /*tp_basicsize*/
        0,                         /*tp_itemsize*/
        0,                         /*tp_dealloc*/
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
        Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
        "Thingamajig",             /* tp_doc */
        0,                         /* tp_traverse */
        0,                         /* tp_clear */
        0,                         /* tp_richcompare */
        0,                         /* tp_weaklistoffset */
        0,                         /* tp_iter */
        0,                         /* tp_iternext */
        PyThingamajig_methods,     /* tp_methods */
        0,                         /* tp_members */
        PyThingamajig_getset,      /* tp_getset */
        PyDoodad::get_type(),    /* tp_base */
        0,                         /* tp_dict */
        0,                         /* tp_descr_get */
        0,                         /* tp_descr_set */
        0,                         /* tp_dictoffset */
        (initproc)PyThingamajig_init,  /* tp_init */
        0,                         /* tp_alloc */
        0,                         /* tp_new */
    };
    return &t;
}

bool PyThingamajig::check(PyObject * p) {
    int r = PyObject_IsSubclass(
        (PyObject*)p->ob_type,
        (PyObject*)PyThingamajig::get_type()
    );
    if (r < 0) {
        PyErr_Clear();
        return false;
    }
    return r;
}

template <typename T>
bool PyThingamajig::sptr_from_python(
    PyObject * p,
    std::shared_ptr<Thingamajig<T>> * s
) {
    std::shared_ptr<Doodad> d;
    if (!PyDoodad::sptr_from_python(p, &d)) {
        return false;
    }
    *s = std::dynamic_pointer_cast<Thingamajig<T>>(d);
    if (!s) {
        PyErr_SetString(PyExc_TypeError, "Incorrect type for Thingamajig.");
        return false;
    }
    return true;
}

template <typename T>
bool PyThingamajig::csptr_from_python(
    PyObject * p,
    std::shared_ptr<extensions::Thingamajig<T> const> * s
) {
    std::shared_ptr<Doodad const> d;
    if (!PyDoodad::csptr_from_python(p, &d)) {
        return false;
    }
    *s = std::dynamic_pointer_cast<Thingamajig<T> const>(d);
    if (!s) {
        PyErr_SetString(PyExc_TypeError, "Incorrect type for Thingamajig.");
        return false;
    }
    return true;
}

#define INSTANTIATE(T) \
    template bool PyThingamajig::sptr_from_python( \
        PyObject * p, \
        std::shared_ptr<Thingamajig<T>> * s \
    ); \
    template bool PyThingamajig::csptr_from_python( \
        PyObject * p, \
        std::shared_ptr<Thingamajig<T> const> * s \
    )

INSTANTIATE(Doodad);
INSTANTIATE(double);

} // namespace extensions

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
initextensions(void) {

    if (PyType_Ready(extensions::PyThingamajig::get_type()) < 0) return;

    typedef extensions::Thingamajig<std::shared_ptr<Doodad>> T1;
    typedef extensions::Thingamajig<double> T2;

    PyDoodad::register_subclass<T1>(extensions::PyThingamajig::get_type());
    PyDoodad::register_subclass<T2>(extensions::PyThingamajig::get_type());

    PyObject* m = Py_InitModule3(
        "extensions", methods,
        "wrappers for classes in extensions.hpp"
    );

    if (!m) return;

    Py_INCREF(extensions::PyThingamajig::get_type());
    PyModule_AddObject(
        m, "Thingamajig",
        (PyObject*)extensions::PyThingamajig::get_type()
    );
}

} // extern "C"
