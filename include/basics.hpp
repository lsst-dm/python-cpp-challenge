#ifndef CHALLENGE_basics_hpp_INCLUDED
#define CHALLENGE_basics_hpp_INCLUDED

#include <string>
#include <memory>

namespace basics {

// Instead of actually creating WhatsIt objects in Python, we'd like to just
// map them to two-element Python tuples, both as arguments and return values.
struct WhatsIt {
    std::string a;
    int b;
};

// A class that should be completely opaque in Python, but can nevertheless
// be round-tripped through the Python layer.
class Secret {
public:

    Secret(Secret const &) = delete;
    Secret(Secret &&) = delete;

    Secret & operator=(Secret const&) = delete;
    Secret & operator=(Secret &&) = delete;

private:

    friend class Doodad;

    friend bool compare(Secret const & a, Secret const & b);
    friend bool adjacent(Secret const & a, Secret const & b);

    Secret();

    std::size_t _index;
};

// A few free functions that compare two Secret objects.
// Must be wrapped, but mostly just to allow Secrets to be used to check other
// aspects of the bindings.
bool compare(Secret const & a, Secret const & b);
bool adjacent(Secret const & a, Secret const & b);

// This should be wrapped as a standard Python type.
// It should have an overloaded constructor, taking WhatsIt (tuple/sequence
// in Python) or str and optionally int.
// We should also define Python comparison (equality and inequality) operators
// that compare by *pointer*, not value.  This will be used by some tests.
// Doodad is noncopyable but moveable; this allows us to guarantee that the
// binding layer isn't doing any unnecessary copies.  We do not expect Python
// bindings to provide access to the move construction or assignment, as there
// is no Python interface for which invalidation of a variable can be
// considered acceptable behavior.
class Doodad {
public:

    // Keyword arguments (without the trailing underscore) should work in
    // Python, as should the default argument.
    explicit Doodad(std::string const & name_, int value_=1);

    // As we map WhatsIt to tuple, this should accept a tuple in Python.
    explicit Doodad(WhatsIt const & it);

    // Copy construction is disabled.
    Doodad(Doodad const &) = delete;
    Doodad& operator=(Doodad const &) = delete;

    // Move construction is allowed, but should not be exposed to Python.
    Doodad(Doodad &&) = default;
    Doodad& operator=(Doodad &&) = default;

    virtual ~Doodad();

    // In Python, the return value should be indistinguishable from an
    // instance created any other way.
    // This option should not require any extra copies beyond the one
    // done in C++.
    virtual std::unique_ptr<Doodad> clone() const;

    // This should accept any (str, int) Python sequence.
    void read(WhatsIt const & it);

    // This should return a (str, int) tuple in Python.
    WhatsIt write() const;

    // Return the opaque Secret object associated with this Doodad.
    Secret const & get_secret() const { return _secret; }

    // Return a shared_ptr to a Doodad that cannot be modified.
    static std::shared_ptr<Doodad const> get_const();

    // We'd like Python properties to be generated for both of these.
    std::string name;
    int value;

private:
    Secret _secret;
};

} // namespace basics

#endif //! CHALLENGE_basics_hpp_INCLUDED
