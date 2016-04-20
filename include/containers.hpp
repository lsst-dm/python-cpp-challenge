#ifndef CHALLENGE_containers_hpp_INCLUDED
#define CHALLENGE_containers_hpp_INCLUDED

#include "basics.hpp"
#include <vector>
#include <map>

namespace containers {

// This class should have a default constructor and copy constructor defined
// in Python, as well as a few special methods for containers (see below).
// It must be wrapped in a way that supports Thingamajigs-as-Doodads when the
// extensions module is imported, but the wrapper code for containers module
// must not #include extensions.hpp.
class DoodadSet {
public:

    // In Python, this should be a class attribute set to the Doodad
    // type object.
    typedef basics::Doodad Item;

    // No need to make a class attribute for this.
    typedef std::vector<std::shared_ptr<Item>>::const_iterator iterator;

    // Transform to __len__ in Python.
    std::size_t size() const { return _items.size(); }

    // Transform to __iter__ in Python (while being careful about dangling
    // pointers.)
    iterator begin() const { return _items.begin(); }
    iterator end() const { return _items.end(); }

    // In Python, this should return as a new list that contains the original
    // items, and should be renamed to "as_list()"".
    std::vector<std::shared_ptr<Item>> as_vector() const { return _items; }

    // In Python, this should accept any Python sequence.
    void assign(std::vector<std::shared_ptr<Item>> const & items) {
        _items = items;
    }

    // This should accept any Python Doodad, including Thingamajigs.
    void add(std::shared_ptr<Item> item);

    // This should accept a (str, int) tuple (or other sequence).
    // There should be a single add_item Python method that accepts both
    // Doodads and tuples.
    void add(basics::WhatsIt const & it);

    // This should be transformed into a Python dict on return, and renamed
    // to "as_dict()".
    std::map<std::string,std::shared_ptr<Item>> as_map() const;

private:
    std::vector<std::shared_ptr<Item>> _items;
};

} // namespace containers


#endif //! CHALLENGE_containers_hpp_INCLUDED
