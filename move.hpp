                          ////
////////////////////////////////////////////////////////
# if !defined move_hpp
# define      move_hpp
# include "coor.hpp"
////////////////////////////////////////////////////////
// +-+-+-+-+-+-+ 0 - Promotion fig( QUEEN=1, ROOK=2, ..)
// |3|2|1|  0  | 1 - Capture, recapture? or not!
// +-+-+-+-+-+-+ 2 - Castles ( dest shows to which side)
//               3 - Ún pá sò
//    e.g.: 000000 - just mouv!
//          100000 - Capture áà pú sá
# define MOVE 0x00
# define PMOT 0x07 
# define CRON 0x08
# define CAST 0x10
# define NPAS 0x20
struct Move { //////////////////////////////////////////
    Coor    dest;
    uint8_t type;
    Move( const Coor& dest, const uint8_t type = MOVE):
        dest( dest), type( type) {}
    bool ismove() const { return !( type & ~PMOT);}
    bool iscron() const { return type & CRON;}
    bool iscast() const { return type & CAST;}
    bool isnpas() const { return type & NPAS;}
    const int get_pmot() const { return type & PMOT;}
    std::string hex() const; // type
    std::string str() const; // -e5, xd8=N, 0-0, ..
};
////////////////////////////////////////////////////////
// log:
# endif ////////////////////////////////////////////////
////                                                ////
