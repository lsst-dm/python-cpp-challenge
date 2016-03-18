%{
#include "py_basics.hpp"
%}

%typemap(out) std::shared_ptr<basics::Doodad> {
    $result = Py<std::shared_ptr<basics::Doodad>>::to_python($1);
}

%typemap(in) basics::Doodad const & (std::shared_ptr<basics::Doodad> tmp) {
    if (!Py<std::shared_ptr<basics::Doodad>>::from_python($input, &tmp)) {
        return nullptr;
    }
    $1 = tmp.get();
}

%typemap(in) basics::Doodad & (std::shared_ptr<basics::Doodad> tmp) {
    if (!Py<std::shared_ptr<basics::Doodad>>::from_python($input, &tmp)) {
        return nullptr;
    }
    $1 = tmp.get();
}

%typemap(in) std::shared_ptr<basics::Doodad> {
    if (!Py<std::shared_ptr<basics::Doodad>>::from_python($input, &$1)) {
        return nullptr;
    }
}

%typemap(in) std::shared_ptr<basics::Doodad const> {
    std::shared_ptr<basics::Doodad> tmp;
    if (!Py<std::shared_ptr<basics::Doodad>>::from_python($input, &tmp)) {
        return nullptr;
    }
    $1 = tmp;
}
