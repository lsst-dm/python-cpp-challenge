#ifndef CHALLENGE_utilities_hpp_INCLUDED
#define CHALLENGE_utilities_hpp_INCLUDED

#include "Python.h"
#include <memory>

namespace utilities {

class InheritanceTree {
public:

    explicit InheritanceTree(PyTypeObject * type);

    PyTypeObject * find(void const * v) const;

    template <typename Base>
    static std::unique_ptr<InheritanceTree> make_root(PyTypeObject * type);

    template <typename Derived, typename Base>
    InheritanceTree * register_subclass(PyTypeObject * type);

    virtual ~InheritanceTree() {}

protected:

    InheritanceTree * _add_child(
        std::unique_ptr<InheritanceTree> child
    );

    InheritanceTree * _add_sibling(
        std::unique_ptr<InheritanceTree> sibling
    );

    virtual bool _try_cast(void const * v) const = 0;

    template <typename Derived, typename Base> class Impl;

    PyTypeObject * _type;
    std::unique_ptr<InheritanceTree> _sibling;
    std::unique_ptr<InheritanceTree> _child;;
};

template <typename Derived, typename Base>
class InheritanceTree::Impl : public InheritanceTree {
public:

    explicit Impl(PyTypeObject * type) : InheritanceTree(type) {}

private:

    virtual bool _try_cast(void const * v) const {
        Base const * b = reinterpret_cast<Base const *>(v);
        return dynamic_cast<Derived const *>(b);
    }

};

template <typename Base>
inline std::unique_ptr<InheritanceTree> InheritanceTree::make_root(
    PyTypeObject * type
) {
    std::unique_ptr<InheritanceTree> r(new Impl<Base,Base>(type));
    return std::move(r);
}

template <typename Derived, typename Base>
inline InheritanceTree * InheritanceTree::register_subclass(
    PyTypeObject * type
) {
    std::unique_ptr<InheritanceTree> r(new Impl<Derived,Base>(type));
    return _add_child(std::move(r));
}


template <typename Base>
struct Py {
    PyObject_HEAD
    bool frozen;
    std::shared_ptr<Base> instance;

    static Py * create(PyTypeObject * type) {
        Py * self = (Py*)type->tp_alloc(type, 0);
        self->frozen = false;
        if (self) {
            new (&self->instance) std::shared_ptr<Base>();
        }
        return self;
    }

    static void destroy(Py * self) {
        typedef std::shared_ptr<Base> Holder;
        self->instance.~Holder();
        self->ob_type->tp_free((PyObject*)self);
    }

};


class IteratorHolder {
public:

    template <typename Converter, typename Iterator>
    static std::unique_ptr<IteratorHolder> create(
        Converter converter, Iterator begin, Iterator end
    );

    virtual PyObject * next() = 0;

    virtual ~IteratorHolder() {}

protected:
    template <typename Converter, typename Iterator> class Impl;
};


template <typename Converter, typename Iterator>
class IteratorHolder::Impl : public IteratorHolder {
public:

    Impl(Converter converter, Iterator begin, Iterator end) :
        _converter(std::move(converter)),
        _current(std::move(begin)),
        _end(std::move(end))
    {}

    virtual PyObject * next() {
        if (_current == _end) {
            return nullptr;
        }
        PyObject * r = _converter(*_current);
        ++_current;
        return r;
    }

private:
    Converter _converter;
    Iterator _current;
    Iterator const _end;
};


template <typename Converter, typename Iterator>
inline std::unique_ptr<IteratorHolder> IteratorHolder::create(
    Converter converter, Iterator begin, Iterator end
) {
    return std::unique_ptr<IteratorHolder>(
        new Impl<Converter,Iterator>(
            std::move(converter), std::move(begin), std::move(end)
        )
    );
}

PyObject * wrapIterator(
    PyObject * owner,
    std::unique_ptr<IteratorHolder> holder
);

template <typename Converter, typename Iterator>
PyObject * wrapIterator(
    PyObject * owner, Converter converter,
    Iterator begin, Iterator end
) {
    return wrapIterator(owner, IteratorHolder::create(converter, begin, end));
}

} // utilities

#endif // !CHALLENGE_utilities_hpp_INCLUDED
