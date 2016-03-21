#include "py_basics.hpp"

namespace basics {

// ----------------------------------------------------------------------------
// WhatsIt
// ----------------------------------------------------------------------------

PyObject * PyWhatsIt::to_python(WhatsIt const & it) {
    return Py_BuildValue(
        "s#i",
        it.a.data(), static_cast<int>(it.a.size()),
        it.b
    );
}

bool PyWhatsIt::from_python(PyObject * p, WhatsIt * it) {
    WhatsIt * result = reinterpret_cast<WhatsIt*>(it);
    int size = 0;
    int b = 0;
    const char * str = nullptr;
    if (!PyArg_ParseTuple(p, "s#i", &str, &size, &b)) {
        return false;
    }
    result->a = std::string(str, size);
    result->b = b;
    return true;
}

// ----------------------------------------------------------------------------
// Secret
// ----------------------------------------------------------------------------

PyObject * PySecret::to_python(Secret const * s) {
    return PyCapsule_New(
        const_cast<Secret*>(s),
        "challenge.basics.Secret",
        nullptr
    );
}

bool PySecret::cptr_from_python(PyObject * p, Secret const ** s) {
    void * v = PyCapsule_GetPointer(p, "challenge.basics.Secret");
    if (!v) {
        return false;
    }
    *s = reinterpret_cast<Secret const*>(v);
    return true;
}

static PyObject * Py_compare(PyObject * self, PyObject * args) {
    Secret * a = nullptr;
    Secret * b = nullptr;
    if (
        !PyArg_ParseTuple(
            args, "O&O&",
            &PySecret::cptr_from_python, &a,
            &PySecret::cptr_from_python, &b
        )
    ) {
        return nullptr;
    }
    bool result = compare(*a, *b);
    return PyBool_FromLong(result);
}

static PyObject * Py_adjacent(PyObject * self, PyObject * args) {
    Secret const * a = nullptr;
    Secret const * b = nullptr;
    if (
        !PyArg_ParseTuple(
            args, "O&O&",
            &PySecret::cptr_from_python, &a,
            &PySecret::cptr_from_python, &b
        )
    ) {
        return nullptr;
    }
    bool result = adjacent(*a, *b);
    return PyBool_FromLong(result);
}

// ----------------------------------------------------------------------------
// Doodad
// ----------------------------------------------------------------------------

static void PyDoodad_dealloc(PyDoodad * self) {
    typedef std::shared_ptr<Doodad> Holder;
    self->instance.~Holder();
    self->ob_type->tp_free((PyObject*)self);
}

static PyDoodad * PyDoodad_new(PyTypeObject * type, PyObject *, PyObject *) {
    PyDoodad * self = (PyDoodad*)type->tp_alloc(type, 0);
    self->frozen = false;
    if (self) {
        new (&self->instance) std::shared_ptr<Doodad>();
    }
    return self;
}

static bool PyDoodad_init_1(PyDoodad * self, PyObject * args, PyObject * kwds) {
    static char const * kwd_names[] = {"name", "value", nullptr};
    int name_size = 0;
    int value = 1;
    const char * name_str = nullptr;
    if (
        PyArg_ParseTupleAndKeywords(
            args, kwds, "s#|i", const_cast<char**>(kwd_names),
            &name_str, &name_size,
            &value
        )
    ) {
        std::string name(name_str, name_size);
        self->instance = std::shared_ptr<Doodad>(
            new Doodad(std::move(name), value)
        );
        return true;
    }
    return false;
}

static bool PyDoodad_init_2(PyDoodad * self, PyObject * args, PyObject * kwds) {
    WhatsIt it;
    if (
        PyArg_ParseTuple(
            args, "O&",
            &PyWhatsIt::from_python, &it
        )
    ) {
        self->instance = std::shared_ptr<Doodad>(new Doodad(it));
        return true;
    }
    return false;
}

static int PyDoodad_init(PyDoodad * self, PyObject * args, PyObject * kwds) {
    if (PyDoodad_init_1(self, args, kwds)) {
        return 0;
    }
    PyErr_Clear();
    if (PyDoodad_init_2(self, args, kwds)) {
        return 0;
    }
    return -1;
}

static PyObject * PyDoodad_clone(PyDoodad * self, PyObject *) {
    if (!self->instance) {
        PyErr_SetString(PyExc_TypeError, "Uninitialized Doodad");
        return nullptr;
    }
    std::shared_ptr<Doodad> copy(self->instance->clone());
    return PyDoodad::to_python(std::move(copy));
}

static PyObject * PyDoodad_read(PyDoodad * self, PyObject * a) {
    if (!self->instance) {
        PyErr_SetString(PyExc_TypeError, "Uninitialized Doodad");
        return nullptr;
    }
    if (self->frozen) {
        PyErr_SetString(PyExc_TypeError, "Cannot modify frozen Doodad.");
        return nullptr;
    }
    WhatsIt it;
    if (!PyWhatsIt::from_python(a, &it)) {
        return nullptr;
    }
    self->instance->read(it);
    Py_RETURN_NONE;
}

static PyObject * PyDoodad_write(PyDoodad * self, PyObject *) {
    if (!self->instance) {
        PyErr_SetString(PyExc_TypeError, "Uninitialized Doodad");
        return nullptr;
    }
    return PyWhatsIt::to_python(self->instance->write());
}

static PyObject * PyDoodad_get_secret(PyDoodad * self, PyObject *) {
    if (!self->instance) {
        PyErr_SetString(PyExc_TypeError, "Uninitialized Doodad");
        return nullptr;
    }
    return PySecret::to_python(&self->instance->get_secret());
}

static PyObject * PyDoodad_get_const(PyObject *, PyObject *) {
    return PyDoodad::to_python(Doodad::get_const());
}

static PyMethodDef PyDoodad_methods[] = {
    {"clone", (PyCFunction)&PyDoodad_clone, METH_NOARGS,
     "Return a copy of the Doodad."},
    {"read", (PyCFunction)&PyDoodad_read, METH_O,
     "Read a WhatsIt into this Doodad."},
    {"write", (PyCFunction)&PyDoodad_write, METH_NOARGS,
     "Write this Doodad into a WhatsIt."},
    {"get_secret", (PyCFunction)&PyDoodad_get_secret, METH_NOARGS,
     "Extract the Secret from this Doodad."},
    {"get_const", (PyCFunction)&PyDoodad_get_const, METH_NOARGS | METH_STATIC,
     "Return a Doodad that cannot be modified."},
    {nullptr}
};

static PyObject * PyDoodad_get_name(PyDoodad * self, void *) {
    if (!self->instance) {
        PyErr_SetString(PyExc_TypeError, "Uninitialized Doodad");
        return nullptr;
    }
    return PyString_FromStringAndSize(
        self->instance->name.data(),
        self->instance->name.size()
    );
}

static int PyDoodad_set_name(PyDoodad * self, PyObject * name, void *) {
    if (!self->instance) {
        PyErr_SetString(PyExc_TypeError, "Uninitialized Doodad");
        return -1;
    }
    if (self->frozen) {
        PyErr_SetString(PyExc_TypeError, "Cannot modify frozen Doodad.");
        return -1;
    }
    Py_ssize_t length = 0;
    char * buffer;
    if (PyString_AsStringAndSize(name, &buffer, &length) < 0) return -1;
    self->instance->name = std::string(buffer, length);
    return 0;
}

static PyObject * PyDoodad_get_value(PyDoodad * self, void *) {
    if (!self->instance) {
        PyErr_SetString(PyExc_TypeError, "Uninitialized Doodad");
        return nullptr;
    }
    return PyInt_FromLong(self->instance->value);
}

static int PyDoodad_set_value(PyDoodad * self, PyObject * value, void *) {
    if (!self->instance) {
        PyErr_SetString(PyExc_TypeError, "Uninitialized Doodad");
        return -1;
    }
    if (self->frozen) {
        PyErr_SetString(PyExc_TypeError, "Cannot modify frozen Doodad.");
        return -1;
    }
    long tmp = PyInt_AsLong(value);
    if (tmp == -1 && PyErr_Occurred()) return -1;
    self->instance->value = tmp;
    return 0;
}

struct PyGetSetDef PyDoodad_getset[] = {
    {
        const_cast<char*>("name"),
        (getter)PyDoodad_get_name,(setter)PyDoodad_set_name,
        const_cast<char*>("name of the Doodad"), nullptr
    }, {
        const_cast<char*>("value"),
        (getter)PyDoodad_get_value, (setter)PyDoodad_set_value,
        const_cast<char*>("value of the Doodad"), nullptr
    }, {
        nullptr
    }
};

PyTypeObject * PyDoodad::get_type() {
    static PyTypeObject t = {
        PyObject_HEAD_INIT(NULL)
        0,                         /*ob_size*/
        "challenge.basics.Doodad", /*tp_name*/
        sizeof(PyDoodad),          /*tp_basicsize*/
        0,                         /*tp_itemsize*/
        (destructor)PyDoodad_dealloc, /*tp_dealloc*/
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
        "Doodad",                  /* tp_doc */
        0,                         /* tp_traverse */
        0,                         /* tp_clear */
        0,                         /* tp_richcompare */
        0,                         /* tp_weaklistoffset */
        0,                         /* tp_iter */
        0,                         /* tp_iternext */
        PyDoodad_methods,          /* tp_methods */
        0,                         /* tp_members */
        PyDoodad_getset,           /* tp_getset */
        0,                         /* tp_base */
        0,                         /* tp_dict */
        0,                         /* tp_descr_get */
        0,                         /* tp_descr_set */
        0,                         /* tp_dictoffset */
        (initproc)PyDoodad_init,   /* tp_init */
        0,                         /* tp_alloc */
        (newfunc)PyDoodad_new,     /* tp_new */
    };
    return &t;
}

bool PyDoodad::check(PyObject * p) {
    int r = PyObject_IsSubclass(
        (PyObject*)p->ob_type,
        (PyObject*)PyDoodad::get_type()
    );
    if (r < 0) {
        PyErr_Clear();
        return false;
    }
    return r;
}

PyObject * PyDoodad::to_python(std::shared_ptr<Doodad> s) {
    PyTypeObject * type = get_tree()->find(s.get());
    PyDoodad * result = PyDoodad_new(type, nullptr, nullptr);
    if (result) {
        result->instance = std::move(s);
    }
    return (PyObject*)result;
}

PyObject * PyDoodad::to_python(std::shared_ptr<Doodad const> s) {
    PyTypeObject * type = get_tree()->find(s.get());
    PyDoodad * result = PyDoodad_new(type, nullptr, nullptr);
    if (result) {
        result->instance = std::const_pointer_cast<Doodad>(std::move(s));
        result->frozen = true;
    }
    return (PyObject*)result;
}

bool PyDoodad::sptr_from_python(PyObject * p, std::shared_ptr<Doodad> * s) {
    if (check(p)) {
        PyDoodad * d = reinterpret_cast<PyDoodad*>(p);
        *reinterpret_cast<std::shared_ptr<Doodad>*>(s) = d->instance;
        if (d->frozen) {
            PyErr_SetString(PyExc_TypeError, "Non-frozen Doodad required.");
            return false;
        }
        return true;
    }
    PyErr_SetString(PyExc_TypeError, "Could not convert object to Doodad.");
    return false;
}

bool PyDoodad::csptr_from_python(
    PyObject * p, std::shared_ptr<Doodad const> * s
) {
    if (check(p)) {
        PyDoodad * d = reinterpret_cast<PyDoodad*>(p);
        *reinterpret_cast<std::shared_ptr<Doodad>*>(s) = d->instance;
        return true;
    }
    PyErr_SetString(PyExc_TypeError, "Could not convert object to Doodad.");
    return false;
}

utilities::InheritanceTree * PyDoodad::get_tree() {
    static std::unique_ptr<utilities::InheritanceTree> tree =
        utilities::InheritanceTree::make_root<Doodad>(get_type());
    return tree.get();
}

} // namespace basics

// ----------------------------------------------------------------------------
// Module Bindings
// ----------------------------------------------------------------------------

static PyMethodDef methods[] = {
    {"compare", &basics::Py_compare, METH_VARARGS,
     "Return true if two Secrets are the same."},
    {"adjacent", &basics::Py_adjacent, METH_VARARGS,
     "Return true if two Secrets were constructed consecutively."},
    {nullptr}
};

extern "C" {

#ifndef PyMODINIT_FUNC  /* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initbasics(void) {
    if (PyType_Ready(basics::PyDoodad::get_type()) < 0) return;

    PyObject* m = Py_InitModule3(
        "basics", methods,
        "wrappers for classes in basics.hpp"
    );

    if (!m) return;

    Py_INCREF(basics::PyDoodad::get_type());
    PyModule_AddObject(m, "Doodad", (PyObject*)basics::PyDoodad::get_type());
}

} // extern "C"
