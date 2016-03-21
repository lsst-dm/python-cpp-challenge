#ifndef CHALLENGE_extensions_hpp_INCLUDED
#define CHALLENGE_extensions_hpp_INCLUDED

#include "basics.hpp"

namespace extensions {

// In Python, we'd like to wrap this as a single Thingamajig type with
// a "dtype" attribute and constructor argument that can be either of
// (double, std::shared_ptr<Doodad>), mimicking the way numpy.ndarray
// handles multiple types.  Actually using NumPy is optional.
template <typename T>
class Thingamajig : public basics::Doodad {
public:

    // Keyword arguments and default values should work in Python.
    Thingamajig(T extra, std::string const & name, int value=1);

    // Copy construction is disabled to ensure bindings don't make unnecessary
    // copies.
    Thingamajig(Thingamajig const &) = delete;
    Thingamajig& operator=(Thingamajig const &) = delete;

    // Move construction is allowed, but is not expected to be exposed to
    // Python.
    Thingamajig(Thingamajig &&) = default;
    Thingamajig& operator=(Thingamajig &&) = default;

    // In Python, this shouldn't have to be wrapped for Thingamajig, as
    // Python inheritance from Doodad should delegate to the Thingamajig
    // implementation anyway.  It should, however, return a Thingamajig
    // instance in Python, not a Doodad that would have to be casted
    // somehow.
    virtual std::unique_ptr<basics::Doodad> clone() const;

    // Return the extra object.
    T get_extra() const { return _extra; }

private:
    T _extra;
};

} // namespace things

#endif //! CHALLENGE_extensions_hpp_INCLUDED
