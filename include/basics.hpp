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

// This should be wrapped as a standard Python type.
// It should have an overloaded constructor, taking any of Doodad, WhatsIt (tuple/sequence in Python),
// or str and optionally int.
class Doodad {
public:

    // Keyword arguments (without the trailing underscore) should work in Python, as should the default
    // argument.
    explicit Doodad(std::string const & name_, int value_=1);

    // This should be overloaded with both the above constructor *and* the (compiler-generated) copy ctor.
    explicit Doodad(WhatsIt const & it);

    virtual ~Doodad() {}

    // In Python, the return value should be indistinguishable from an instances created any other way,
    // and should not be a copy (beyond the one done in the C++ code);
    virtual std::unique_ptr<Doodad> clone() const;

    // This should accept any (str, int) Python sequence.
    void read(WhatsIt const & it);

    // This should return a (str, int) tuple in Python.
    WhatsIt write() const;

    // We'd like Python properties to be generated for both of these.
    std::string name;
    int value;

    // This should be easy to wrap - we just want it to test other aspects of
    // the wrapping
    static std::size_t get_instance_count();

};

} // namespace basics

#endif //! CHALLENGE_basics_hpp_INCLUDED
