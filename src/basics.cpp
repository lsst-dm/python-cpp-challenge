#include "basics.hpp"

namespace basics {

namespace {

std::size_t & internal_secret_count() {
    static std::size_t count = 0;
    return count;
}

} // anonymous

Secret::Secret() : _index(++internal_secret_count()) {}

bool compare(Secret const & a, Secret const & b) {
    return a._index == b._index;
}

bool adjacent(Secret const & a, Secret const & b) {
    return a._index + 1u == b._index;
}

Doodad::Doodad(std::string const & name_, int value_) :
    name(name_), value(value_)
{}

Doodad::Doodad(WhatsIt const & it) : name(it.a), value(it.b) {}

Doodad::~Doodad() {}

std::unique_ptr<Doodad> Doodad::clone() const {
    return std::unique_ptr<Doodad>(new Doodad(name, value));
}

void Doodad::read(WhatsIt const & it) {
    name = it.a;
    value = it.b;
}

WhatsIt Doodad::write() const {
    WhatsIt it = {name, value};
    return it;
}

std::shared_ptr<Doodad const> Doodad::get_const() {
    static std::shared_ptr<Doodad const> instance(new Doodad("frozen", 50));
    return instance;
}

} // namespace basics
