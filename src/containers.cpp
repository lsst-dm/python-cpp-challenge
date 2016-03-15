#include "containers.hpp"

namespace containers {

void DoodadSet::add_item(std::shared_ptr<Item> item) {
    _items.push_back(std::move(item));
}

void DoodadSet::add_item(basics::WhatsIt const & it) {
    std::shared_ptr<basics::Doodad> doodad(new basics::Doodad(it));
    _items.push_back(std::move(doodad));
}

std::map<std::string,std::shared_ptr<DoodadSet::Item>>
DoodadSet::as_map() const {
    std::map<std::string,std::shared_ptr<Item>> result;
    for (auto const & item : _items) {
        result[item->name] = item;
    }
    return result;
}

} // namespace containers
