///////////////////////////////////////////////////////*
///////////////////////////////////////////////////////~
# include <thread>
# include <numeric>
///////////////////////////////////////////////////////=
# include "Node.hpp"
# include "Move.hpp"
///////////////////////////////////////////////////////_
static const int NFMOVES{ 80 }; 
//////////////////////////////////////////////////[Node]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////>
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
    node->Elevate( Casl::Rytes( vec[ 2 ]));
    if( vec[ 3 ] != "-" ){
        node->enPassant = Board::GetOffset( vec[ 3 ]);
    }
    return node;
}
///////////////////////////////////////////////////////_
Node::~Node(){
#   ifdef DEBUG
    std::cout << "Node out\n";
#   endif
}
///////////////////////////////////////////////////////_
void Node::Elevate( rytes_t rytes ){
    const rytes_t oldRytes{ this->rytes };
    this->rytes = rytes;
    rytes ^= oldRytes;
    for( int j{}; rytes; rytes >>= 1, ++j ){
        if( rytes & 1 ){
            const color_t c{ Casl::C( j )};
            const flank_t f{ Casl::F( j )};
            KingGen* const kingGen {
                (KingGen* )army[ c ].king->gen
            };
            CaslGen* const caslGen{ kingGen->casl[ f ]};
            caslGen->Subscribe();
        }
    }    
}
///////////////////////////////////////////////////////_
void Node::DeElevate( rytes_t rytes ){
    // this->rytes don't need to be updated, they have
    // been flip-flopped by the Update method. rytes are
    // the old rytes, check vhere to unsubscribe.
    rytes ^= this->rytes;
    for( int j{}; rytes; rytes >>= 1, ++j ){
        if( rytes & 1 ){
            const color_t c{ Casl::C( j )};
            const flank_t f{ Casl::F( j )};
            KingGen* const kingGen {
                ( KingGen* )army[ c ].king->gen
            };
            CaslGen* const caslGen{ kingGen->casl[ f ]};
            caslGen->Unsubscribe();
        }
    }    
}
////////////////////////////////////////////////////////
Unit * Node::InsertCoin
( const char c, const int i, const int j ){
    const color_t color{ Casl::Color( c )};
    const fig_t fig{ GetFig( c )};
    const offset_t k{ Board::GetOffset( i, j )};
    Unit * u{ army[ color ].insert( fig, color, this )};
    board.Land( u, k );
    return u;
}
///////////////////////////////////////////////////////=
void Node::Perft( const int depth ){
    const std::string fen{ FEN()};
    std::vector<std::thread> threads;
    std::vector<u_64> results;
    std::vector<Move> movs;
    movs.reserve( NFMOVES );
    GetMoves( movs );
    const auto nfmovs{ movs.size()};
    std::vector<Node *> copy{ nfmovs, nullptr };
    for( std::size_t j{}; j < nfmovs; ++j ){
        copy[ j ] = cons( fen );
        copy[ j ]->MakeMove( movs[ j ]);
        threads.emplace_back([ copy, j, depth, &results ](){
            u_64 n{ copy[ j ]->Perft_( depth - 1 )};
            results.push_back( n );
        });
    }
    for( auto &t: threads ){ t.join(); }
    for( auto p: copy ){ delete p; }
    copy.clear();
    u_64 total{
        std::accumulate( results.begin(), results.end(), 0LLU )
    };
    std::cout << total << endl;
}
///////////////////////////////////////////////////////_
u_64 Node::Perft_( const int depth ){
    if( !depth ){
        return 1;
    }
    std::vector<Move> movs;
    movs.reserve( NFMOVES );
    u_64 nfMoves{};
    GetMoves( movs );
    for( const Move& mv: movs ){
        MakeMove( mv );
        if( !Check()){
            nfMoves += Perft_( depth - 1 );
        }
        UndoMove( mv );
    }
    return nfMoves;
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
void Node::MakeMove( const Move& mov ){
    const auto copyryte{ this->rytes }; // DeElevate
    enPassant = 0; // reset
    const move_t casl{ mov.GetCASL()};
    if( casl ){
        MakeCasl( casl );
    } else {
        const move_t pmot{ mov.GetPMOT()};
        Unit * unit;
        if( pmot ){
            unit = board.Promote( mov.GetPmotFig( pmot ),
                                  mov.src, mov.dst );
        } else {
            unit = board.Travel( mov.src, mov.dst );
        }
        ////////////////////////////////////////////////
        // npas and not cron is double fyorst move
        if( mov.IsNPAS() and mov.IsCRON()){
            unit = board.LiftOff(
                mov.dst + Board::FORWARD[ !theSwitch ]);
        }
        // CRON test
        if( unit ){
            army[ unit->GetColor()].unlink( unit );
            unit->gen->Unsubscribe();
        } else { // NPAS update( thats )
            if( mov.IsNPAS()){
                enPassant = mov.src +
                    Board::FORWARD[ theSwitch ];
            }
        }
    }
    FlipTheSwitch();
    DeElevate( copyryte );
}
///////////////////////////////////////////////////////=
void Node::UndoMove( const Move& mov ){
    move_t casl{ mov.GetCASL()};
    if( casl ){
        MakeCasl( casl, true );
    } else {
        Unit * unit;
        if( mov.GetPMOT()){
            unit = board.Promote( PAWN, mov.dst, 
                                        mov.src );
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
}///////////////////////////////////////////////////////=
std::ostream&
operator<<( std::ostream& os, Node const * const node ){
    return os << node->board
              << Board::GetCoord( node->enPassant )
              << " " << Casl::FEN( node->rytes )
              << endl
              << node->army[  node->theSwitch ] 
              << node->army[ !node->theSwitch ];
}
///////////////////////////////////////////////////////_
bool Node::Check() const {
    const offset_t k{
        army[ !theSwitch ].king->GetOffset()
    };
    return board.Check( k, theSwitch );
}
////////////////////////////////////////////////////////
std::string Node::FEN() const {
    std::stringstream ss;
    ss << board.FEN() << " "
       << "bw"[ theSwitch ] << " "
       << Casl::FEN( rytes ) << " "
       << (enPassant? Board::GetCoord( enPassant ): "-")
       << " 0 1";
    return ss.str();
}
///////////////////////////////////////////////////////_
//
