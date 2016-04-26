%{
#include "Python.h"
#include "basics.hpp"
#include "basics.h"
%}

%typemap(out) std::shared_ptr<basics::Doodad> {
    $result = newDoodadFromSptr($1);
}

%typemap(out) std::shared_ptr<basics::Doodad const> {
    $result = newImmutableDoodadFromCsptr($1);
}

%typemap(in) basics::Doodad const & (std::shared_ptr<basics::Doodad const> tmp) {
    if (!csptrFromImmutableDoodad($input, &tmp)) {
        PyErr_Clear();
        std::shared_ptr<basics::Doodad> tmp2;
        if (!sptrFromDoodad($input, &tmp2)) {
            return nullptr;
        }
        tmp = tmp2;
    }
    $1 = const_cast<basics::Doodad*>(tmp.get());
}

%typemap(in) basics::Doodad & (std::shared_ptr<basics::Doodad> tmp) {
    if (!sptrFromDoodad($input, &tmp)) {
        return nullptr;
    }
    $1 = tmp.get();
}

%typemap(in) std::shared_ptr<basics::Doodad> {
    if (!sptrFromDoodad($input, &$1)) {
        return nullptr;
    }
}

%typemap(in) std::shared_ptr<basics::Doodad const> {
    std::shared_ptr<basics::Doodad const> tmp;
    if (!csptrFromImmutableDoodad($input, &tmp)) {
        PyErr_Clear();
        std::shared_ptr<basics::Doodad> tmp2;
        if (!sptrFromDoodad($input, &tmp2)) {
            return nullptr;
        }
        tmp = tmp2;
    }
    $1 = tmp;
}
