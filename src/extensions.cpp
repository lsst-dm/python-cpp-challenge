#include "extensions.hpp"
#include <complex>

namespace extensions {

// The copy() functions here are a workaround for the lack of a copy
// constructor on Doodad.
namespace {

basics::Doodad copy(basics::Doodad const & d) {
    return basics::Doodad(d.name, d.value);
}

template <typename T>
T copy(T const & t) {
    return t;
}

} // anonymous

template <typename T>
Thingamajig<T>::Thingamajig(T extra, std::string const & name, double value) :
    basics::Doodad(name, value), _extra(copy(extra)) {}

template <typename T>
std::unique_ptr<basics::Doodad> Thingamajig<T>::clone() const {
    return std::unique_ptr<Thingamajig>(
        new Thingamajig(
            copy(_extra),
            this->name,
            this->value
        )
    );
}

template class Thingamajig<std::complex<double>>;
template class Thingamajig<basics::Doodad>;

} // namespace things
