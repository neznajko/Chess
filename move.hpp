////////////////////////////////////////////////////////
#if !defined move_hpp
#define move_hpp

#include <forward_list>

#include "coor.hpp"
////////////////////////////////////////////////////////
// +-+-+-+-+-+-+ 0 - Promotion figure( QUEEN=1, ROOK=2, .. )
// |3|2|1|  0  | 1 - Capture, recapture? or not!
// +-+-+-+-+-+-+ 2 - Castles ( dest shows to which side )
//               3 - Ún pá sò
//    e.g.: 000000 - just mouv!
//          100000 - Capture áà pú sá
#define PMOT 0x07 
#define CRON 0x08
#define CAST 0x10
#define NPAS 0x20
class Move {
private:
    Coor   _sors;
    Coor   _dest;
    int8_t _type;
public:
    Move( const Coor& sors,
          const Coor& dest,
          const int8_t type=0 ):
        _sors( sors ), _dest( dest ), _type( type ) {}
    bool isMove() const { return !( _type & ~PMOT ); }
    bool isCron() const { return _type & CRON; }
    bool isCast() const { return _type & CAST; }
    bool isNpas() const { return _type & NPAS; }
    const int getPmot() const { return _type & PMOT; }
    const Coor& getSors() const { return _sors; }
    const Coor& getDest() const { return _dest; }
    std::string hex() const; // _type
    const int8_t getType() const { return _type; }
};
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& os, const Move& move );

typedef std::forward_list<Move> moves_t;
////////////////////////////////////////////////////////
// log:
#endif// move_hpp
////////////////////////////////////////////////////////
