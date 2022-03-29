                          ////
////////////////////////////////////////////////////////
# if !defined node_hpp
# define      node_hpp
# include "figure.hpp"
# include <vector>
////////////////////////////////////////////////////////
// Here board is extended with a frame to handle Knight
// jumps out of the box etc. The actual chess board is
// named DOJO( place of the Way).
constexpr int BOARDZE{ FRAMEWID * 2 + RANKS }; // size
constexpr int DOJOBEGIN{ FRAMEWID};
constexpr int DOJOEND{ BOARDZE - FRAMEWID};
////////////////////////////////////////////////////////
//  +  0 1 2 3 4 5 6 7 8 9 a b  (j)
//  0  1 1 1 1 1 1 1 1 1 1 1 1
//  1  1 1 1 1 1 1 1 1 1 1 1 1
//  2  1 1 r n b q k b n r 1 1
//  3  1 1 p p p p p p p p 1 1
//  4  1 1 0 0 0 0 0 0 0 0 1 1
//  5  1 1 0 0 0 0 0 0 0 0 1 1
//  6  1 1 0 0 0 0 0 0 0 0 1 1
//  7  1 1 0 0 0 0 0 0 0 0 1 1
//  8  1 1 P P P P P P P P 1 1
//  9  1 1 R N B Q K B N R 1 1
//  a  1 1 1 1 1 1 1 1 1 1 1 1
//  b  1 1 1 1 1 1 1 1 1 1 1 1
// (i)
////////////////////////////////////////////////////////
struct Board {
    Unit* u[ BOARDZE][ BOARDZE];
    std::string str() const; // for hashing
    Board(); // cons
    Unit* getUnit( const Coor& coor) const {
        return u[ coor.i][ coor.j];
    }
    Unit* setUnit( const Coor& coor, Unit* v);
};
std::ostream&
operator<<( std::ostream& ostrm, const Board& board);
////////////////////////////////////////////////////////
struct Army { // dll( ring)
    const static std::string NAME[];
    Unit* king;
    std::vector<Unit*> bench; // out of play units
    u_int cntrs[ NOFIGS] = {}; // figure counters
    Army(): king( new Unit()) {
        king->prev = king->next = king;
    }
    Unit* insert( Figure* f);
    void unlink( Unit* p, const bool del=false);
    void getmoz();
    void dmpmoz();
    ~Army();
};
std::ostream&
operator<<( std::ostream& ostrm, const Army& army);
////////////////////////////////////////////////////////
struct Node { // Chess Position
    bool active_color; // side to move
    Board board;
    Army army[ RED]; // Black and White
    Coor npas; // on-pa-so square
    Node( const std::string& fen); // FEN position
};
////////////////////////////////////////////////////////
// log: THINK
# endif ////////////////////////////////////////////////
////                                                ////
