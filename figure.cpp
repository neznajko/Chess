////////////////////////////////////////////////////////
#include <cctype> // isupper

#include "figure.hpp"
#include "stuff.hpp"
////////////////////////////////////////////////////////
const std::string Figure::_UNITS[] = {
    "kqrbnp", "KQRBNP" };
// Pawn is 4 units, King's value should be irrelevant,
// but is possible to make an implementation where the
// King is like a common figure.
const int Figure:: _VALUES[] = {
    800, 36, 20, 14, 12, 4
};
Figure::Figure( const char c, const int i, const int j ):
    _coor( i, j )
{
    const int color{ 0 < isupper( c )}; // boom
    
    _type = _UNITS[ color ].find( c );
    if( !color ) _type = ~_type; // BLACK
}
////////////////////////////////////////////////////////
int Figure::getUnit() const
{
    return getColor() ? _type : ~_type;
}
char Figure::getChar() const
{
    return _UNITS[ getColor() ][ getUnit() ];
}
void Figure::pmot( const int unit )
{
    _type = getColor() ? unit : ~unit;
}
////////////////////////////////////////////////////////
std::ostream& 
operator<<( std::ostream& os, const Figure& fig )
{
    return os << fig.getChar(), fig._coor;
}
std::string Figure::getPix() const
{
    static const std::string PIX[] = {
        " ♚ ", " ♛ ", ",'`" , "(+)", " % ", " o "
    };
    return PIX[ getUnit() ];
}
////////////////////////////////////////////////////////
// log: make namespace units or something            []
//
