#include "extensions.hpp"
#include <complex>

namespace extensions {

template <typename T>
Thingamajig<T>::Thingamajig(T extra, std::string const & name, int value) :
    basics::Doodad(name, value), _extra(extra) {}

template <typename T>
std::unique_ptr<basics::Doodad> Thingamajig<T>::clone() const {
    return std::unique_ptr<Thingamajig>(
        new Thingamajig(
            _extra,
            this->name,
            this->value
        )
    );
}

template class Thingamajig<double>;
template class Thingamajig<std::shared_ptr<basics::Doodad>>;

} // namespace things
