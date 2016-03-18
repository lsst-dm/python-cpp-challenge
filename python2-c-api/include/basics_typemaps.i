%{
#include "py_basics.hpp"
%}

%typemap(out) std::shared_ptr<basics::Doodad> {
    $result = Py<basics::Doodad>::to_python($1);
}

%typemap(out) std::shared_ptr<basics::Doodad const> {
    $result = Py<basics::Doodad>::to_python($1);
}

%typemap(in) basics::Doodad const &
    (std::shared_ptr<basics::Doodad const> tmp)
{
    if (!Py<basics::Doodad>::csptr_from_python($input, &tmp)) {
        return nullptr;
    }
    // const_cast below shouldn't be necessary; it's a Swig weirdness.
    $1 = const_cast<basics::Doodad*>(tmp.get());
}

%typemap(in) basics::Doodad & (std::shared_ptr<basics::Doodad> tmp) {
    if (!Py<basics::Doodad>::sptr_from_python($input, &tmp)) {
        return nullptr;
    }
    $1 = tmp.get();
}

%typemap(in) std::shared_ptr<basics::Doodad> {
    if (!Py<basics::Doodad>::sptr_from_python($input, &$1)) {
        return nullptr;
    }
}

%typemap(in) std::shared_ptr<basics::Doodad const> {
    std::shared_ptr<basics::Doodad const> tmp;
    if (!Py<basics::Doodad>::csptr_from_python($input, &tmp)) {
        return nullptr;
    }
    $1 = tmp;
}
