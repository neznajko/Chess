////////////////////////////////////////////////////////
# ifndef MOVE_HPP
# define MOVE_HPP
////////////////////////////////////////////////////////
# include "defs.hpp"
# include "io.hpp"
////////////////////////////////////////////////////////
//////////////////////////////////////////////////[Move]
////////////////////////////////////////////////////////
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
struct Move { // Define no constructors here
    move_t type; 
    offset_t src;  
    offset_t dst;  
    offset_t enPassant; // backups for move undoes
    rytes_t rytes;      //
    move_t GetPMOT() const {
        static const int PMOT_MASK{ 0x1e }; // 0001 1110
        return type & PMOT_MASK;
    }
    bool IsCRON() const {
        return ( type & CRON ) > 0;
    }
    bool IsNPAS() const {
        return ( type & NPAS ) > 0;
    }
    move_t GetCASL() const {
        static const int CASL_MASK{ 0x60 }; // 0110 0000
        return type & CASL_MASK;
    }
    fig_t GetPmotFig( move_t pmot ) const;
    // e4-e5, f7xd8=N, 0-0, ..
    std::string GetStr( const bool long_fmt=false ) const;
 };
////////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream& ostrm,
                          const Move& mv );
////////////////////////////////////////////////////////
# endif
///////////////////////////////////////////////////////_
