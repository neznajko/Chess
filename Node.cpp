////////////////////////////////////////////////////////
# include "Node.hpp"
# include "Move.hpp"
//////////////////////////////////////////////////[Node]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
std::vector<std::string>
Node::SplitFEN( const std::string& fen ){
    std::stringstream ss( fen );
    std::string buf;
    std::vector<std::string> vec;
    while( ss >> buf ){
        vec.push_back( std::move( buf ));
    }    
    return vec;
}
////////////////////////////////////////////////////////
// Forsyth–Edwards Notation
// After 1.e4 c5:
// rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR
// w KQkq c6 0 2
////////////////////////////////////////////////////////
Node* Node::cons( const std::string& fen ){
    Node * const node = new Node();
    int i{ Board::FRAME_HEIGHT };
    int j{ Board::FRAME_WIDTH };
    auto vec{ SplitFEN( fen )};
    // Elevate here to update casl counters.
    node->Elevate( Casl::Rytes( vec[ 2 ]));
    for( const char c: vec[ 0 ]){
        if( c == '/' ){ // new row
            j = Board::FRAME_WIDTH; // reset column
            ++i;                       // increment row
        } else if( std::isdigit( c )){ // empty sqares
            j += c - '0'; // rewind
        } else {
            node->InsertCoin( c, i, j++ );
        }
    }
    if( vec[ 1 ].front() == 'w' ){
        node->FlipTheSwitch();
    }
    if( vec[ 3 ] != "-" ){
        node->enPassant = Board::GetOffset( vec[ 3 ]);
    }
    return node;
}
///////////////////////////////////////////////////////_
void Node::Elevate( rytes_t rytes ){
    const rytes_t oldRytes{ this->rytes };
    this->rytes = rytes;
    rytes ^= oldRytes;
    // consider checking in !rytes: return
    for( int j{}; rytes; rytes >>= 1, ++j ){
        if( rytes & 1 ){
            const color_t c{ Casl::C( j )};
            const flank_t f{ Casl::F( j )};
            KingGen * const kingGen {
                ( KingGen * )army[ c ].king->gen
            };
            CaslGen * const caslGen{
                kingGen->casl[ f ]
            };
            caslGen->Subscribe();
        }
    }    
}
///////////////////////////////////////////////////////_
// No need to update current rytes, they have been 
// changed eventualy by the Casl Generator. rytes are
// the old rytes before makeing a move.
void Node::Delevate( rytes_t rytes ){
    rytes ^= this->rytes;
    // if rytes is 0, than nothing has changed
    if( !rytes ){
        return;
    }
    // this is the same code as above, only Usubscribe
    for( int j{}; rytes; rytes >>= 1, ++j ){
        if( rytes & 1 ){
            const color_t c{ Casl::C( j )};
            const flank_t f{ Casl::F( j )};
            KingGen * const kingGen {
                ( KingGen * )army[ c ].king->gen
            };
            CaslGen * const caslGen{
                kingGen->casl[ f ]
            };
            caslGen->Unsubscribe();
        }
    }    
}
////////////////////////////////////////////////////////
Unit * Node::InsertCoin( const char c,
                         const int i,
                         const int j ){
    const color_t color{ Casl::Color( c )};
    const fig_t fig{ GetFig( c )};
    const offset_t k{ Board::GetOffset( i, j )};
    Unit * u{ army[ color ].insert( fig, color, this )};
    board.Land( u, k );
    return u;
}
///////////////////////////////////////////////////////=
void Node::GetMoves( std::vector<Move>& movs ) const {
    Unit * const king{ army[ theSwitch ].king };
    Unit * unit{ king };
    do {
        unit->gen->GetMoves( movs );
        unit = unit->next;
    } while( unit != king );
}
////////////////////////////////////////////////////////
void Node::MakeCasl( const move_t casl,
                     const bool undo ){
    const color_t c{ undo ?! theSwitch : theSwitch };
    const flank_t f{ casl == SHOTCASL };
    const offset_t king_src{
        Casl::KING_FROM[ c ]
    };
    const offset_t king_dst{
        Casl::KING_TO[ c ][ f ]
    };
    const offset_t rook_src{
        Casl::ROOK_FROM[ c ][ f ]
    };
    const offset_t rook_dst{
        Casl::ROOK_TO[ c ][ f ]
    };
    if( undo ){
        board.Travel( king_dst, king_src );
        board.Travel( rook_dst, rook_src );
    } else {
        board.Travel( king_src, king_dst );
        board.Travel( rook_src, rook_dst );
    }
}
////////////////////////////////////////////////////////
void Node::MakeMove( const Move& mov ){
    // copY casl rytes here, so later de-elevate.
    const rytes_t rytes{ this->rytes };
    enPassant = 0;
    move_t casl{ mov.GetCASL()};
    if( casl ){
        MakeCasl( casl );
    } else {
        const move_t pmot{ mov.GetPMOT()};
        Unit * unit;
        if( pmot ){
            const auto fig{ mov.GetPmotFig( pmot )};
            unit = board.Promote( fig, mov.src, mov.dst );
        } else {
            unit = board.Travel( mov.src, mov.dst );
        }
        if( mov.IsNPAS() and mov.IsCRON()){
            // get the pawn, bcoz unit now is NIL
            const offset_t k = mov.dst +
                Board::FORWARD[ !theSwitch ];
            unit = board.LiftOff( k );
        }
        // CRON test
        if( unit ){
            army[ unit->GetColor()].unlink( unit );
            unit->gen->Unsubscribe();
        } else { // NPAS update
            if( mov.IsNPAS()){
                enPassant = mov.src +
                    Board::FORWARD[ theSwitch ];
            }
        }
    }
    FlipTheSwitch();
    Delevate( rytes );
}
////////////////////////////////////////////////////////
void Node::UndoMove( const Move& mov ){
    move_t casl{ mov.GetCASL()};
    if( casl ){
        MakeCasl( casl, true );
    } else {
        Unit * unit;
        if( mov.GetPMOT()){
            unit = board.Promote( PAWN, mov.dst, mov.src );
        } else {
            unit = board.Travel( mov.dst, mov.src );
        }
        if( mov.IsCRON()){
            unit = army[ theSwitch ].dance();
            board.Land( unit, unit->GetOffset());
        }
    }
    FlipTheSwitch();
    // re-establish stuff
    enPassant = mov.enPassant;
    Elevate( mov.rytes );
}
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& os, Node const * const node ){
    return os << node->board
              << Board::GetCoord( node->enPassant )
              << " " << Casl::FEN( node->rytes )
              << endl
              << node->army[  node->theSwitch ] 
              << node->army[ !node->theSwitch ];
}
////////////////////////////////////////////////////////
u_64 Node::Perft( const int depth ){
    static const int NF_MOVES{ 256 };
    if( !depth ){
        return 1;
    }
    std::vector<Move> movs;
    movs.reserve( NF_MOVES );
    u_64 nfMoves{};
    GetMoves( movs );
    for( const Move& mv: movs ){
        MakeMove( mv );
        if( !Check()){
            nfMoves += Perft( depth - 1 );
        }
        UndoMove( mv );
    }
    return nfMoves;
}
///////////////////////////////////////////////////////_
bool Node::Check() const {
    const offset_t k{
        army[ !theSwitch ].king->GetOffset()
    };
    return board.Check( k, theSwitch );
}
////////////////////////////////////////////////////////
//
