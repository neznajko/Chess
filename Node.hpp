////////////////////////////////////////////////////////
# ifndef NODE_HPP
# define NODE_HPP
////////////////////////////////////////////////////////
# include "Army.hpp"
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//////////////////////////////////////////////////[Node]
////////////////////////////////////////////////////////
# include <map>
////////////////////////////////////////////////////////
typedef unsigned long long u_64;
////////////////////////////////////////////////////////
class Node {
    Board board;
    Army army[ RED ];
    color_t theSwitch{ BLACK };
    static fig_t GetFig( const char c ){
        static const std::map<char,fig_t> M = {
            {'k', KING },
            {'q', QUEEN },
            {'r', ROOK },
            {'b', BISHOP },
            {'n', KNIGHT },
            {'p', PAWN },
        };
        return M.at( std::tolower( c ));
    }
    void MakeCasl( const move_t casl,
                   const bool undo=false );
public:
    offset_t enPassant{};
    rytes_t rytes{};
    static std::vector<std::string>
    SplitFEN( const std::string& fen );
    static Node* cons( const std::string& fen );
    Node():
        army{
            Army( new Unit( KING, BLACK, this )),
            Army( new Unit( KING, WHITE, this )),
        } {}
    Board& GetBoard() {
        return board;
    }
    void FlipTheSwitch() {
        theSwitch = !theSwitch;
    }
    void Elevate( rytes_t rytes );
    Unit * InsertCoin( const char c,
                       const int i,
                       const int j );
    u_64 Perft( const int depth );
    void GetMoves( std::vector<Move>& movs ) const;
    void MakeMove( const Move& mov );
    void UndoMove( const Move& mov );
    friend std::ostream&
    operator<<( std::ostream& os,
                Node const * const node );
    // Check vhether passive king is in check.
    bool Check() const;
};
////////////////////////////////////////////////////////
# endif
////////////////////////////////////////////////////////
