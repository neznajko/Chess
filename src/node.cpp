////////////////////////////////////////////////////////////////
# include "iterator.hpp"
////////////////////////////////////////////////////////////////
# include <sstream>
# include <map>
# include <cctype>
////////////////////////////////////////////////////////////////
const int Board::dR[] = {
    -WIDTH-1, -WIDTH+0, -WIDTH+1,
          -1,                 +1,
    +WIDTH-1, +WIDTH+0, +WIDTH+1,
};
std::string Board::coor( int k ){
    int i = geti( k ) - FRAME_HEIGHT;
    int j = getj( k ) - FRAME_WIDTH;
    if( i < 0 or i >= DOJO_SIZE ) return "?";
    if( j < 0 or j >= DOJO_SIZE ) return "?";
    std::string s{ "87654321"[ i ]};
    return "abcdefgh"[ j ] + s;
}
// 5 = = 5 =                 = = = =   e4: j = 'e' - 'a' + 4
// 6 = = 4 =                 = = = =       i = 8 - 4 + 2
int Board::getk( const std::string& sqr ){
    char file{ sqr[0] }; // e
    char rank{ sqr[1] }; // 4
    int j{ file - 'a' + FRAME_WIDTH };
    int i{ DOJO_SIZE - ( rank - '0' ) + FRAME_HEIGHT };
    return getk( i, j );
}
int Board::npas_convyort( int npas, color_t acolor ){
    if( acolor == BLACK ){
        return npas - WIDTH;
    } else {
        return npas + WIDTH;
    }
}
////////////////////////////////////////////////////////////////
Board::Board() {
    for( int k = 0; k < SIZE; k++ ){
        setgrd( k );   
    }
    for( int i = 0; i < DOJO_SIZE; i++ ){
        for( int j = 0; j < DOJO_SIZE; j++ ){
            setnil( getk( i + FRAME_HEIGHT, j + FRAME_WIDTH ));
        }
    }
}
////////////////////////////////////////////////////////////////
void Board::dump() {
    for( int i = 0; i < HEIGHT; i++ ){
        for( int j = 0; j < WIDTH; j++ ){
            std::cout << " " << getunit( i, j )->getchar();
        }
        std::cout << endl;
    }
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////[ Army ]
const std::string Army::NAME[ RED ] = { "Black", "White" };
//
Unit* Army::insert( fig_t fig, int k ){
    Unit* u{ king };
    if( fig == Figure::KING ){
        u->k = k;
    } else {
        u = new Unit( fig, k );
        u->next = king->next;
        king->next = u;
        u->next->prev = u;
        u->prev = king;
    }
    cntrs[ fig ]++;
    return u;
}
Army::~Army() {
    while( king->next != king ){
        unlink( king->next, true );
    }
    delete king;
    for( Unit* p: bench ){
        delete p;
    }
}
////////////////////////////////////////////////////////////////
//   >next>    >next>                                    :before
// n        p         q       
//   <prev<    <prev<
////////////////////////////////////////////////////////////////
//   >next>   ' bench:            >next>                  :after
// n        q |        n        p        q
//   <prev<   |          <prev<
////////////////////////////////////////////////////////////////
void Army::unlink( Unit* p, const bool del ){
    p->prev->next = p->next;
    p->next->prev = p->prev;
    if( del ){
        delete p;
    } else {
        bench.push_back( p );
        cntrs[ p->fig ]--;
    }
}
Unit* Army::dance() {
    Unit* p{ bench.back() };
    bench.pop_back();
    cntrs[ p->fig ]++;
    p->prev->next = p->next->prev = p;
    return p;
}
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& ostrm, const Army& army ){
    Unit* u{ army.king };
    do {
        u = u->next;
        u->itor->reset( u->k ); // u->reset_itor();
        ostrm << *u << ": " << u->itor << endl;
    } while( u != army.king );
    ostrm << "Bench: " << army.bench;
    ostrm << "\nCntrs: " 
          << std::vector<int>( army.cntrs, army.cntrs + Figure::NIL );
    return ostrm;
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////[ Node ]
void Node::setunit( color_t color, fig_t fig, int k ){
    Unit* u{ army[ color ].insert( fig, k )};
    brd[ color ].setunit( u, k );
    switch( fig ){
        case Figure::KING:
        {
            u->itor = new KingIterator( this );
            break;
        }
        case Figure::ROOK:
        {
            u->itor = new RookIterator( this );
            break;
        }
        case Figure::KNIGHT:
        {
            u->itor = new KnightIterator( this );
            break;
        }
        default: break;
    }
}
void Node::paint(){
    static const std::string DARK_SQR{ "195;58;20" };
    static const std::string LYTE_SQR{ "230;150;18" };
    static const std::string DARK_FIG{ "0;0;0" };
    static const std::string LYTE_FIG{ "220;250;255" };
    static const Brush DARK[] = {
        Brush( DARK_FIG, DARK_SQR ),
        Brush( LYTE_FIG, DARK_SQR ),
    };
    static const Brush LYTE[] = {
        Brush( DARK_FIG, LYTE_SQR ),
        Brush( LYTE_FIG, LYTE_SQR ),
    };
    for( int i = 0; i < Board::DOJO_SIZE; i++ ){
        for( int j = 0; j < Board::DOJO_SIZE; j++ ){
            int i1{ i + Board::FRAME_HEIGHT };
            int j1{ j + Board::FRAME_WIDTH };
            std::string c{ " "};
            color_t color{ BLACK };
            Unit* u{ brd[ color ].getunit( i1, j1 )};
            if( u->fig == Figure::NIL ){
                // because there are two boards
                color = WHITE;
                u = brd[ color ].getunit( i1, j1 );
            }
            c += std::string( 1, u->getchar()) + " ";
            if(( j^i ) & 1 ){ // don't ask me how I figured that out
                std::cout << DARK[ color ].patron( c );
            } else {
                std::cout << LYTE[ color ].patron( c );
            }
        }
        std::cout << endl;
    }
}
void Node::dump() {
    paint();
    std::cout << Army::NAME[ flip_the_switch ]
              << " to move, npas( " << Board::coor( npas ) << " )" << endl
              << "Move History: " << move_history << endl
              << "Npas History: " << npas_history << endl
              << army[ flip_the_switch ] << endl
              << cast << endl;
}
////////////////////////////////////////////////////////////////
// What is the convntion for the npas square? Let's say white to
// move and has a pawn on e5, last Black mv was f7-f5, and Vhite
// can take on f6, so naturally f6 is the npas square, but let's
// say the convention is f5, and it is passed as dest mov square.
void Node::do_the_npas( int sorc, int dest ){
    // perform a re-capture
    re_capture( dest );
    // adjust dest square
    auto color{ GetActvColor() };
    if( color == WHITE ){
        dest -= Board::WIDTH;
    } else {
        dest += Board::WIDTH;
    }
    travel( sorc, dest );
}
void Node::undo_the_npas( int dest, int sorc ){ ////////////////
    // Vhite has just moved e5 capture ón pà sò d6, in our new
    // convention that means the move was sorc:e5, dest:d5, now
    // it is Black to move and they have to calculate the
    // correct landing square, could you help them?:)
    auto color{ GetActvColor() };
    if( color == BLACK ){
        dest -= Board::WIDTH;
    } else {
        dest += Board::WIDTH;
    }
    // That will dance the links and put back the pawn on the
    // board.
    re_uncapture();
    // The capturing pawn travel back in time.
    untravel( dest, sorc );
}
void Node::make_move( const Move& mv ){ ////////////////////////
    npas_history.push_back( npas );
    move_history.push_back( mv );
    Move::type_t iscast{ mv.get_cast() };
    if( iscast > 0 ){
        if( iscast == LONG_CAST ) cast.make_cast( QUEENSIDE );
        else                      cast.make_cast(  KINGSIDE );
    } else if( mv.is_npas() ){
        do_the_npas( mv.sorc, mv.dest );
    } else {
        Move::type_t pmot{ mv.get_pmot() };
        if( pmot > 0 ){
            // change sorc figure
            Unit* u{ actv().getunit( mv.sorc )};
            u->fig = mv.get_pmot_fig( pmot );
        }
        if( mv.is_cron() ){
            re_capture( mv.dest );
        }
        travel( mv.sorc, mv.dest );
    }
    flip_the_switch = !flip_the_switch;
}
void Node::re_capture( int k ){
    Unit* u{ pasv().getunit( k )};
    army[ !flip_the_switch ].unlink( u );
    pasv().setnil( k );
}
////_////////////////////-//////////////////////////////////////
void Node::re_uncapture() {
    Unit* p{ army[ flip_the_switch ].dance() };
    actv().setunit( p, p->k );
}
void Node::travel( int sorc, int dest ){
    Unit* u{ actv().liftoff( sorc )};
    ++u->noofmoos;// reset castlings for king and rook moves
    actv().land( u, dest );
    // reset npas here
    npas = 0;
    static const int PAWN_START_RANK[] = { 3, 8 };
    if( u->fig == Figure::PAWN ){
        int sorc_rank{ Board::geti( sorc )};
        int dest_rank{ Board::geti( dest )};
        if( std::abs( dest_rank - sorc_rank ) > 1 ){
            npas = dest;
        }
    }
}
////////////////////////////////////////////////////////////////
void Node::untravel( int dest, int sorc ){
    // Let's say White has moved e4-e5, and now it is Black move,
    // that is we have to unmove e5-e4 vhen Black is active side.
    Unit* u{ pasv().liftoff( dest )};
    --u->noofmoos;// Ok
    pasv().land( u, sorc );
}
void Node::unmake_move(){
    // log: What if stacks are empty?
    // ans: Segmentation fault!
    Move mv{ move_history.back() };
    move_history.pop_back();
    Move::type_t iscast{ mv.get_cast() };
    if( iscast > 0 ){
        if( iscast == LONG_CAST ) cast.unmake_cast( QUEENSIDE );
        else                      cast.unmake_cast(  KINGSIDE );
    } else if( mv.is_npas() ){
        undo_the_npas( mv.dest, mv.sorc );
    } else {
        if( mv.get_pmot() > 0 ){
             // unpromote
             Unit* u{ pasv().getunit( mv.dest )};
             u->fig = Figure::PAWN;
        }
        if( mv.is_cron()) re_uncapture();
        untravel( mv.dest, mv.sorc );
    }
    // re-establish the npas square, after traveling and stuff
    npas = npas_history.back();
    npas_history.pop_back();
    flip_the_switch = !flip_the_switch;
}
///////////////////////////////////////////////////////////////_
// split string into words
std::vector<std::string> split( const std::string& str)
{
    std::stringstream ss( str);
    std::string buf;
    std::vector<std::string> vec;
    while( ss >> buf) {
        vec.push_back( std::move( buf));
    }    
    return vec;
}
////////////////////////////////////////////////////////////////
fig_t FEN::getfig( char c ){
    static const std::map<char,fig_t> m = {
        {'k', Figure::KING},
        {'q', Figure::QUEEN},
        {'r', Figure::ROOK},
        {'b', Figure::BISHOP},
        {'n', Figure::KNIGHT},
        {'p', Figure::PAWN},
    };
    return m.at( std::tolower( c));
}
////////////////////////////////////////////////////////////////
// Forsyth–Edwards Notation
// After 1.e4 c5:
// rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR
// w KQkq c6 0 2
////////////////////////////////////////////////////////////////
Node* FEN::cons( const std::string& fen ){
    Node* node = new Node();
    int i{ Board::FRAME_HEIGHT };
    int j{ Board::FRAME_WIDTH };
    auto vec{ split( fen)};
    for( const char c: vec[ 0]) {
        if( c == '/') { // new row
            j = Board::FRAME_WIDTH; // reset column
            ++i;                    // increment row
        } else if( std::isdigit( c)) { // empty sqares
            j += c - '0'; // rewind
        } else {
            color_t color = ( color_t )( isupper( c) != 0 );
            fig_t fig = getfig( c);
            int k = Board::getk( i, j);
            node->setunit( color, fig, k);
            j++;
        }
    }
    node->flip_the_switch = vec[ 1].front() == 'w';
    for( auto color: { BLACK, WHITE }){
        node->cast.setmoos( vec[ 2 ], color );
    }
    // This is the orginal definition of the npas square, zo it
    // must be adjusted.
    if( vec[ 3 ] != "-" ){
        node->npas = Board::npas_convyort( Board::getk( vec[ 3 ]),
                                           node->GetActvColor());
    }
    return node;
}
////////////////////////////////////////////////////////////////
//
