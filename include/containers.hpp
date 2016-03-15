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

    // In Python, this should return as a list that contains the original items.
    std::vector<std::shared_ptr<Item>> get_items() const { return _items; }

    // In Python, this should accept any Python sequence.
    void set_items(std::vector<std::shared_ptr<Item>> const & items) {
        _items = items;
    }

    // This should accept any Python Doodad, including Thingamajigs.
    void add_item(std::shared_ptr<Item> item);

    // This should accept a (str, int) tuple (or other sequence).
    // There should be a single add_item Python method that accepts both
    // Doodads and tuples.
    void add_item(basics::WhatsIt const & it);

    // Ideally, if the returned object is actually a Thingamajig, it should
    // be returned as one.
    std::shared_ptr<Item> get_item(std::size_t index) const {
        return _items[index];
    }

    // This should be transformed into a Python dict on return.
    std::map<std::string,std::shared_ptr<Item>> as_map() const;

private:
    std::vector<std::shared_ptr<Item>> _items;
};

} // namespace containers


#endif //! CHALLENGE_containers_hpp_INCLUDED
