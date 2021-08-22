#if !defined position_hpp
#define position_hpp

#include <vector>

#include "figure.hpp"
#include "cast.hpp"
////////////////////////////////////////////////////////
const int BSIZE{ 2 * FRAMEW + NRANKS }; // board size
class Board {
public:
    // Figure offsets from Position::_fig vectors,
    // complemented for Black. 16 and 17 are empty and
    // guard squares respectively.
    int8_t offset[ BSIZE ][ BSIZE ];
    Board();
    int get_offset( const Coor& coor ) const; // I knew it!
    int abs_offset( const Coor& coor ) const;
    int8_t& operator[]( const Coor& coor );
};
#define EMPTY 16
#define GUARD 17

std::ostream&
operator<<( std::ostream& os, const Board& board );
////////////////////////////////////////////////////////
struct Score {
    Move move;
    int  eval;
};
////////////////////////////////////////////////////////
typedef std::forward_list<Move>
( Position::*UnitMoves )( const Figure& ) const;
////////////////////////////////////////////////////////
class Position { // 200 bytes
private:
    const static UnitMoves _UM[];
    std::vector<Figure> _fig[ RED ];
    bool    _active_color;
    Board   _board;
    Coor    _npas;
    Castles rytes_;
public:
    Position( const std::string& fen );
    int eval() const;
    std::forward_list<Position> fork() const;
    std::vector<Score> getScores( const int depth) const;
    friend std::ostream&
    operator<<( std::ostream& os, const Position& pos );
    bool get_active_color() const { return _active_color; }
    void makemove( const Move& move );
    const Board& get_board() const { return _board; }
    void ordeer() const;
    void spit_moves() const;
    //
    moves_t KnightMoves( const Figure& fig ) const;
    moves_t RangeMoves( const Figure& fig,
                        const int start,
                        const int step ) const;
    moves_t RookMoves( const Figure& fig ) const;
    moves_t BishopMoves( const Figure& fig ) const;
    moves_t QueenMoves( const Figure& fig ) const;
    moves_t KingMoves( const Figure& fig ) const;
    moves_t PawnMoves( const Figure& fig ) const;
    moves_t getMoves( const Figure& fig ) const;
    //
    bool undafire( const Coor& coor) const;
    std::size_t hash() const;
    bool ck() const;
    bool ckmove( const Move& move ) const;
    void pass(){ _active_color = !_active_color;}
    void debug_spit() const; // no colored output
    int flipflop( const int depth) const;
};
////////////////////////////////////////////////////////
// log: - move makemove to Command                   [x]
#endif// position_hpp
////////////////////////////////////////////////////////
