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


} // utilities

#endif // !CHALLENGE_utilities_hpp_INCLUDED
