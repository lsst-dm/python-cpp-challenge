#ifndef CHALLENGE_extensions_hpp_INCLUDED
#define CHALLENGE_extensions_hpp_INCLUDED

#include "basics.hpp"

namespace extensions {

// In Python, we'd ideally like to wrap this as a single Thingamajig type with
// a "dtype" attribute and constructor argument that can be either of
// (std::complex<double>, Doodad).
// But since that's hard, we recommend starting with just support for
// std::complex<double>, which should be sufficient to pass many tests.
template <typename T>
class Thingamajig : public basics::Doodad {
public:

    // Keyword arguments and default values should work in Python.
    Thingamajig(T extra, std::string const & name, double value=1);

    // Copy construction is disabled to ensure bindings don't make unnecessary
    // copies.
    Thingamajig(Thingamajig const &) = delete;
    Thingamajig& operator=(Thingamajig const &) = delete;

    // Move construction is allowed, but is not expected to be exposed to
    // Python.
    Thingamajig(Thingamajig &&) = default;
    Thingamajig& operator=(Thingamajig &&) = default;

    // In Python, this should ideally return a Thingamajig instance, not just
    // a Doodad.
    virtual std::unique_ptr<basics::Doodad> clone() const;

    // When T is complex<float> these will always return a copy, since they'll
    // be returning a Python built-in.  When returning a Doodad, we'd ideally
    // like to propagate constness, and tie the lifetime of the Doodad to the
    // lifetime of the Thingamajig, so it's impossible to get a dangling
    // reference in Python.
    T const & get_extra() const { return _extra; }
    T & get_extra() { return _extra; }

private:
    T _extra;
};

} // namespace things

#endif //! CHALLENGE_extensions_hpp_INCLUDED
