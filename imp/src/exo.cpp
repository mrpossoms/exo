#include "exo.hpp"

using namespace exo;


std::ostream& msg::operator<<(std::ostream& os, const Hdr& h)
{
    return os << h.type << h.magic << h.payload_length;
}


std::istream& msg::operator>>(std::istream& is, Hdr& h)
{
    return is >> h.type >> h.magic >> h.payload_length;
}


msg::Hdr::Hdr(std::istream& is)
{
    is >> *this;
}


msg::Hdr msg::Hdr::from_stream(std::istream& is)
{
    return { is };
}


// Result msg::Payload::seek_set(size_t pos);

Mod::Mod(ID&& name)
{
    _name = name;
}
/*
bool Mod::msg_compatible(msg::Hdr& h) { return false; }
Result Mod::msg_recieved(msg::Hdr& h, msg::Payload& p) { return {}; }
Result Mod::enter()  { return {}; }
Result Mod::update() { return {}; }
Result Mod::exit()   { return {}; }
*/
