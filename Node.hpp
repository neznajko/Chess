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
    // Perform castling and un-castling moves: : : : : :
    void MakeCasl( const move_t casl,
                   const bool undo=false );
public://///////////////////////////////////////////////
    offset_t enPassant{};
    rytes_t rytes{};
    // Split space separated FEN string into fields, , ,
    static std::vector<std::string> SplitFEN
    ( const std::string& fen );
    // Construct a Node from Forsyth-Edwards Notation- -
    static Node* cons( const std::string& fen );
    Node():
        army{
            Army( new Unit( KING, BLACK, this )),
            Army( new Unit( KING, WHITE, this )),
        }{
#       ifdef DEBUG
        std::cout << "Node cons\n";
#       endif        
    }
    ~Node();
    Board& GetBoard(){ return board; }
    void FlipTheSwitch(){ theSwitch = !theSwitch; }
    // Subscribe for every new ryte ,`,`,`,`,`,`,`,`,`,`,
    void Elevate( rytes_t rytes );
    // Compares rytes and this->rytes and unsubscribe : :
    void DeElevate( rytes_t rytes );
    // Inserts a figure into the position / / / / / / / /
    Unit* InsertCoin
    ( const char c, const int i, const int j );
    // Multi thread performance test = - _ - = + = - _ -
    void Perft( const int depth );
    // Single thread performance test function : : : : :
    u_64 Perft_( const int depth );
    // Get active army moves _ _ _ _ _ _ _ _ _ _ _ _ _ _
    void GetMoves( std::vector<Move>& movs ) const;
    // Make mov move , , , , , , , , , , , , , , , , , ,
    void MakeMove( const Move& mov );
    // Undoing mov _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
    void UndoMove( const Move& mov );
    // Dumps stuff, mainly for debugging . . . . . . . .
    friend std::ostream& operator<<
    (std::ostream& os, Node const * const node );
    // Is the passive king in check? -------------------
    bool Check() const;
    // get FEN position ////////////////////////////////
    std::string FEN() const;
};
///////////////////////////////////////////////////////_
# endif
////////////////////////////////////////////////////////
