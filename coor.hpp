                          ////
////////////////////////////////////////////////////////
# if !defined coor_hpp
# define      coor_hpp
# include "io.hpp"
# include <cstdint> // int8_t
////////////////////////////////////////////////////////
constexpr int FRAMEWID{ 2};  // frame width
constexpr int RANKS{ 8};     // number of ranks
constexpr int FILES{ RANKS}; // number of files
////////////////////////////////////////////////////////
struct Coor { // Board Coordinates
    enum{ NW, N, NE, E, SE, S, SW, W, WORLD}; // cw
    static const Coor dR[ WORLD + 1];
    static const Coor dN[ WORLD]; // kNight moves
    int8_t i; // reversed rank
    int8_t j; // file position
    Coor( const int8_t i = 0, const int8_t j = 0):
        i( i), j( j) {}
    Coor( const std::string& chessnot);
    inline int rank() const;
    inline int file() const;
    std::string chessnot() const;
    void operator+=( const Coor& y) {
        i += y.i;
        j += y.j;
    }
    void operator-=( const Coor& y) {
        i -= y.i;
        j -= y.j;
    }
    bool operator==( const Coor& y) const {
        return( i == y.i and j == y.j);
    }
    bool operator!=( const Coor& y) const {
        return( i != y.i or j != y.j);
    }
};
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& ostrm, const Coor& coor);
Coor operator+( const Coor& x, const Coor& y);
Coor operator-( const Coor& x, const Coor& y);
# endif ////////////////////////////////////////////////
// log:
////////////////////////////////////////////////////////
    ////                                        ////
