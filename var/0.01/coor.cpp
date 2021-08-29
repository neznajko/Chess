////////////////////////////////////////////////////////
#include "coor.hpp"

Coor::Coor( const std::string& chessnot )
{
    _i = FRAMEW + NRANKS - ( chessnot[1] - '0' );
    _j = FRAMEW + ( chessnot[0] - 'a' );
}
std::ostream&
operator<<( std::ostream& os, const Coor& coor )
{
    return os << coor.chessnot();
}
int Coor::getRank() const
{
    return FRAMEW + NRANKS - _i;
}
int Coor::getFile() const
{
    return _j - FRAMEW + 1;
}
std::string Coor::chessnot() const
{
    const int f{ getFile() };
    const int r{ getRank() };
    if( f < 1 or f > 8 or r < 1 or r > 8 )
    {
        return "?!";
    }
    return "=abcdefgh"[f] + std::to_string( r );
}
Coor operator+( const Coor& x, const Coor& y )
{
    return Coor( x.get_i() + y.get_i(),
                 x.get_j() + y.get_j() );
}
void Coor::operator+=( const Coor& y )
{
    _i += y.get_i();
    _j += y.get_j();
}
bool Coor::operator==( const Coor& y ) const
{
    return _i == y.get_i() and _j == y.get_j();
}
bool Coor::operator!=( const Coor& y ) const
{
    return _i != y.get_i() or _j != y.get_j();
}
////////////////////////////////////////////////////////
// log:
