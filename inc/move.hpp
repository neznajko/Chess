                          ////
////////////////////////////////////////////////////////
# ifndef MOVE_HPP
# define MOVE_HPP
////////////////////////////////////////////////////////
# include "fig.hpp"
////////////////////////////////////////////////////////
// +---+---+---+---+---+---+---+---+
// | P | L | S | N | B | R | Q | C |
// +---+---+---+---+---+---+---+---+
// C - Capture Recapture Or Not, if this bit is set it's
// capture or recapture, otherwise it's not.
// Q,R,B,N - Queen, Rook, Bishop, kNight promotion.
// S,L - Short and Long Castles.
// P - capture òn pá só
////////////////////////////////////////////////////////
constexpr int MOVE{             0 };
constexpr int CRON{        1 << 0 };
constexpr int QUEEN_PMOT{  1 << 1 };
constexpr int ROOK_PMOT{   1 << 2 };
constexpr int BISHOP_PMOT{ 1 << 3 };
constexpr int KNIGHT_PMOT{ 1 << 4 };
constexpr int SHORT_CAST{  1 << 5 };
constexpr int LONG_CAST{   1 << 6 };
constexpr int NPAS{        1 << 7 };
struct Move { //////////////////////////////////////////
    static const Move Fin;
    //
    typedef int type_t;
    type_t type;
    int    sorc;
    int    dest;
    // This is becoz of castle moves vhere sorc and dest
    // are irrelevant.
    Move( type_t type_, int sorc_=0, int dest_=0 ):
        type( type_ ), sorc( sorc_ ), dest( dest_ )
    {}
    type_t get_pmot() const {
        static const int MASK{ 0x1e }; // 0001 1110
        return type & MASK;
    }
    bool is_cron() const {
        return ( type & CRON ) > 0;
    }
    type_t get_cast() const {
        static const int MASK{ 0x60 }; // 0110 0000
        return type & MASK;
    }
    bool is_npas() const {
        return ( type & NPAS ) > 0;
    }
    fig_t get_pmot_fig( type_t pmot ) const;
    std::string str() const; // e4-e5, f7xd8=N, 0-0, ..
};
////////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream& os, const Move& mv );
// log:
# endif/////////////////////////////////////////////////
////                                                ////
