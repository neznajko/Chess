////////////////////////////////////////////////////////
#if !defined coor_hpp
#define coor_hpp

#include <cstdint> // int8_t

#include <string>
#include <ostream>

#define FRAMEW 2 // frame width
#define NRANKS 8 // number of ranks
////////////////////////////////////////////////////////
class Coor { // Board Coordinates
private:
    int8_t _i; // reversed rank
    int8_t _j; // file position
public:
    Coor( const std::string& chessnot );
    Coor( const int8_t i=0, const int8_t j=0 ):
        _i( i ), _j( j ) {}
    int  get_i() const { return _i; }
    int  get_j() const { return _j; }
    void set_i( const int8_t i ){ _i = i; }
    void set_j( const int8_t j ){ _j = j; }
    int  getRank() const;
    int  getFile() const;
    std::string chessnot() const;
    void operator+=( const Coor& y );
    bool operator==( const Coor& y ) const;
    bool operator!=( const Coor& y ) const;
};
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& os, const Coor& coor );
Coor operator+( const Coor& x, const Coor& y );
////////////////////////////////////////////////////////
// log:
#endif// coor_hpp
////////////////////////////////////////////////////////
