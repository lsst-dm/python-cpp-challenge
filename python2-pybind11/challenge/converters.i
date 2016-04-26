%module(package="challenge") converters

%{
#include "basics.hpp"
#include <pybind11/pybind11.h>

/* Needed for casting to work with shared_ptr<Doodad> */
PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
%}

%include "std_string.i"

// Challenge solutions must provide only the following file, made available
// on the Swig include path.  It should contain the necessary Swig typmaps to
// convert a Doodad C++ object to Python and back, for all of the functions
// below.
%include "basics_typemaps.i"

%init %{
    /* Get registered types from other pybind11 modules */
    pybind11::detail::get_internals();
%}

%inline %{

std::shared_ptr<basics::Doodad> make_sptr(
    std::string const & name, int value
) {
    return std::shared_ptr<basics::Doodad>(new basics::Doodad(name, value));
}

std::shared_ptr<basics::Doodad const> make_csptr(
    std::string const & name, int value
) {
    return std::shared_ptr<basics::Doodad const>(
        new basics::Doodad(name, value)
    );
}

bool accept_ref(
    basics::Doodad & d,
    std::string const & name, int value
) {
    return d.name == name && d.value == value;
}

bool accept_cref(
    basics::Doodad const & d,
    std::string const & name, int value
) {
    return d.name == name && d.value == value;
}

bool accept_sptr(
    std::shared_ptr<basics::Doodad> d,
    std::string const & name, int value
) {
    return d->name == name && d->value == value;
}

bool accept_csptr(
    std::shared_ptr<basics::Doodad const> d,
    std::string const & name, int value
) {
    return d->name == name && d->value == value;
}

%}
