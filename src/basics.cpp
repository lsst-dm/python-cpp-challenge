#include "basics.hpp"

namespace basics {

static std::size_t & internal_instance_count() {
    static std::size_t count = 0;
    return count;
}

Doodad::Doodad(std::string const & name_, int value_) :
    name(name_), value(value_)
{
    ++internal_instance_count();
}

Doodad::Doodad(WhatsIt const & it) : name(it.a), value(it.b) {}

std::unique_ptr<Doodad> Doodad::clone() const {
    return std::unique_ptr<Doodad>(new Doodad(*this));
}

void Doodad::read(WhatsIt const & it) {
    name = it.a;
    value = it.b;
}

WhatsIt Doodad::write() const {
    WhatsIt it = {name, value};
    return it;
}

std::size_t Doodad::get_instance_count() {
    return internal_instance_count();
}

} // namespace basics
