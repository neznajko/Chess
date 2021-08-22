////////////////////////////////////////////////////////
#include <bitset>

#include "move.hpp"
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& os, const Move& move )
{
    return os << move.getSors() << ' '
              << move.getDest() << ' '
              << move.hex();
}
std::string Move::hex() const
{
    return "0x" + std::bitset<8>( _type ).to_string();
}
////////////////////////////////////////////////////////
// log:
