                          ////
////////////////////////////////////////////////////////
# include "coor.hpp"
////////////////////////////////////////////////////////
// WORLD DIRECTIONS ( i, j)
// NW(-1,-1) N(-1, 0) NE(-1, 1)
//  W( 0,-1)           E( 0, 1)
// SW( 1,-1) S( 1, 0) SE( 1, 1)
const Coor Coor::dR[] = {
//  NW           N            NE           E
    Coor(-1,-1), Coor(-1, 0), Coor(-1, 1), Coor( 0, 1),
//  SE           S            SW           W
    Coor( 1, 1), Coor( 1, 0), Coor( 1,-1), Coor( 0,-1),
};
// Knight moves
// +---+---+---+---+---+
// |   | 2 |   | 3 |   | -2
// +---+---+---+---+---+
// | 1 |   |   |   | 4 | -1
// +---+---+---+---+---+
// |   |   |   |   |   |  0
// +---+---+---+---+---+
// | 8 |   |   |   | 5 | +1
// +---+---+---+---+---+
// |   | 7 |   | 6 |   | +2
// +---+---+---+---+---+
//  -2  -1   0  +1  +2
const Coor Coor::dN[] = {
//  1             2             3             4
    Coor(-1,-2), Coor(-2,-1), Coor(-2, 1), Coor(-1, 2),
//  5             6             7             8
    Coor( 1, 2), Coor( 2, 1), Coor( 2,-1), Coor( 1,-2),
};
////////////////////////////////////////////////////////
Coor::Coor( const std::string& chessnot) // a4
{
    i = FRAMEWID - ( chessnot[ 1] - '0') + RANKS;
    j = FRAMEWID + ( chessnot[ 0] - 'a');
}
int Coor::rank() const
{
    return( FRAMEWID - i + RANKS);
}
int Coor::file() const
{
    return( j + 1 - FRAMEWID);
}
std::string Coor::chessnot() const
{
    if( rank() < 1 or rank() > RANKS or
        file() < 1 or file() > FILES) return "?!";
    
    return( "=abcdefgh"[ file()] +
            std::to_string( rank()));
}
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& ostrm, const Coor& coor)
{
    return( ostrm << coor.chessnot());
}
Coor operator+( const Coor& x, const Coor& y)
{
    return Coor( x.i + y.i, x.j + y.j);
}
Coor operator-( const Coor& x, const Coor& y)
{
    return Coor( x.i - y.i, x.j - y.j);
}
////////////////////////////////////////////////////////
// log:
////////////////////////////////////////////////////////
    ////                                        ////
