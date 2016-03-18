%module(package="challenge") converters

%{
#include "basics.hpp"
%}

%include "std_string.i"

// Challenge solutions must provide only the following file, made available
// on the Swig include path.  It should contain the necessary Swig typmaps to
// convert a Doodad C++ object to Python and back, for all of the functions
// below.
%include "basics_typemaps.i"

%inline %{

std::shared_ptr<basics::Doodad> make_sptr(std::string const & name, int value) {
    return std::shared_ptr<basics::Doodad>(new basics::Doodad(name, value));
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
