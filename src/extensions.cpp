#include "extensions.hpp"
#include <complex>

namespace extensions {

template <typename T>
Thingamajig<T>::Thingamajig(T extra, std::string const & name, double value) :
    basics::Doodad(name, value), _extra(extra) {}

template <typename T>
std::unique_ptr<basics::Doodad> Thingamajig<T>::clone() const {
    return std::unique_ptr<Thingamajig>(new Thingamajig(*this));
}

template class Thingamajig<std::complex<float>>;
template class Thingamajig<basics::Doodad>;

} // namespace things
