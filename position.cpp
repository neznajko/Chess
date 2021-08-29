////////////////////////////////////////////////////////
#include "position.hpp"
#include "stuff.hpp"

#include <climits>
#include <cstring>    // memset

#include <sstream> 
#include <iostream>
#include <iomanip>    // setw
#include <iterator>   // istring_iterator
#include <deque>
#include <functional> // hash
#include <unordered_map>
#include <algorithm>
////////////////////////////////////////////////////////
namespace glob {
    std::unordered_map<std::size_t, int> lookup;
}
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& os, const Board& board )
{
    std::stringstream ss;
    
    for( int i = 0; i < BSIZE; ++i ){
        for( int j = 0; j < BSIZE; ++j )
        {
            ss << std::setw( 3 )
               << static_cast<int>( board.offset[i][j] );
        }
        if( i < BSIZE - 1 ) ss << '\n';
    }
    return os << ss.str();
}
Board::Board()
{
    static const size_t n{ BSIZE * BSIZE };
    
    memset( offset, GUARD, n ); // fill with Guards
}
int8_t& Board::operator[]( const Coor& coor )
{
    return offset[ coor.get_i() ][ coor.get_j() ];
}
int Board::get_offset( const Coor& coor ) const
{
    return offset[ coor.get_i() ][ coor.get_j() ];
}
int Board::abs_offset( const Coor& coor ) const
{
    const int f{ get_offset( coor )};

    return f < 0 ? ~f : f;
}
////////////////////////////////////////////////////////
// split string into words
std::vector<std::string> split( const std::string& str )
{
    std::stringstream ss( str );
    using it = std::istream_iterator<std::string>;
    
    return std::vector<std::string>( it( ss ), it() );
}
////////////////////////////////////////////////////////
Position::Position( const std::string& fen )
// Forsyth–Edwards Notation
// After 1.e4 c5:
// rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR
// w KQkq c6 0 2
{
    int i{ FRAMEW };
    int j{ i };
    // Kings at front, but pushing_back other figures,
    // to correctly compute the offsets use these flags.
    bool headsoff[] = { true, true };
    std::vector<std::string> vec{ split( fen )};
    for( const char c: vec[0] )
    {
        if( c == '/' )
        {
            j = FRAMEW; // reset column
            ++i;        // increment row
        } else if( isdigit( c )) {
            const size_t n{( size_t ) c - '0' };
            memset( &_board.offset[i][j], EMPTY, n );
            j += n;
        } else {
            int offset;
            const int y{ 0 < isupper( c )};
            std::vector<Figure>& a{ _fig[y] };
            if( c == 'k' or c == 'K' )
            {
                a.emplace( a.begin(), c, i, j );
                headsoff[y] = false;
                offset = 0;
            } else {
                offset = a.size() + headsoff[y];
                a.emplace_back( c, i, j );
            }
            _board.offset[i][j] = y ? offset: ~offset;
            ++j;
        }}
    _active_color = vec[1].front() == 'w';
    rytes_.setFenRytes( vec[2] );
    // 0h! pä 'sø
    if( vec[3] != "-" ) _npas = Coor( vec[3] ); // copy
}
////////////////////////////////////////////////////////
void Position::ordeer() const
{
    std::stringstream ss;
    std::string armyName[] = { "Black", "White" };
    for( int color: { BLACK, WHITE })
    {
        ss << " ", armyName[ color ], ":\n ";
        const std::vector<Figure>& fig{ _fig[ color ]};
        const int n{ static_cast<int>( fig.size() )};

        for( int j = 0; j < n; ++j )
        {
            if( (j + 1) % 8 ) ss << fig[j], ", ";
            else              ss << fig[j], "\n ";
        }
        ss.seekp( -2, ss.cur );
        ss << " \n";
    }
    std::cout << ss.str();
}
////////////////////////////////////////////////////////
std::ostream& 
operator<<( std::ostream& os, const Position& pos )
{
    static const Brush GNRL[] = {
        Brush( "100;0;0", "50;20;20" ),
        Brush( "255;0;0", "50;20;20" ),
        Brush( "0;255;0", "50;20;20" )
    };
    static const Brush MPTY_DRK[] = {
        Brush( "20;50;40", "29;30;21" ),
        Brush( "50;120;140", "51;103;135" )
    };
    static const Brush MPTY_LIT[] = {
        Brush( "60;50;40", "70;60;50" ),
        Brush( "90;140;250", "100;150;200" )
    };
    static const Brush FIGR_LIT[][RED] = {
        {
            Brush( "10;8;6", "71;60;49" ),
            Brush( "20;25;30", "99;155;202" )
        },{
            Brush( "150;140;130", "72;61;50" ),
            Brush( "180;220;200", "101;160;198" )
        }};    
    static const Brush FIGR_DRK[][RED] = {
        {
            Brush( "0;0;0", "28;35;23" ),
            Brush( "0;0;0", "53;105;130" )
        },{
            Brush( "220;215;235", "30;25;20" ),
            Brush( "255;255;255", "50;100;140" )
        }};    
    std::stringstream ss;
    std::string buff{ " ? " };
    for( int i = 0; i < BSIZE; ++i ){
        // k is alternating white and black squares.
        int k{ !( i & 1 )};
        for( int j = 0; j < BSIZE; ++j, k = !k )
        {
            int f{ pos._board.offset[i][j] };
            if( f == GUARD ){
                // tag the ranks and files
                if( j == 1 && 1 < i && i < 10 )
                {
                    buff[1] = "87654321"[ i - 2 ];
                    ss << GNRL[1].patron( buff  );
                } else if( i == 10 && 1 < j && j < 10 ){
                    buff[1] = 'a' + j - 2;
                    ss << GNRL[1].patron( buff );
                } else { // click - clock
                    if( pos.get_active_color() )
                    {
                        if( j == 11 and i == 9 )
                        {
                            ss << GNRL[2].patron( " = " );
                        } else {
                            ss << GNRL[0].patron( "===" );
                        }                        
                    } else {
                        if( j == 11 and i == 2 )
                        {
                            ss << GNRL[2].patron( " = " );
                        } else {
                            ss << GNRL[0].patron( "===" );
                        }}}
            } else if( f == EMPTY ) {
                if( i < 4 or i > 7 )
                {
                    ss << MPTY_DRK[k].patron( " . " );
                } else {
                    ss << MPTY_LIT[k].patron( " . " );
                }
            } else {
                // [ n e v e r ] use sprintf
                const bool y{ f > -1 }; // ,,
                if( y == BLACK ) f = ~f;
                const Figure& fig{ pos._fig[y][f] };
                const std::string pix{ fig.getPix() };
                if( fig.isOutOfPlay() )
                {
                    if( i < 4 or i > 7 )
                    {
                        ss << MPTY_DRK[k].patron( " . " );
                    } else {
                        ss << MPTY_LIT[k].patron( " . " );
                    }
                } else {
                    if( i < 4 or i > 7 )
                    {
                        ss << FIGR_DRK[y][k].patron( pix );
                    } else {
                        ss << FIGR_LIT[y][k].patron( pix );
                    }
                }
            }
        }
        ss << '\n';
    }
    return os << ss.str();
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//
//  +  0 1 2 3 4 5 6 7 8 9 a b  (j)
//
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
//
// (i)
//
////////////////////////////////////////////////////////
// WORLD DIRECTIONS ( i, j )
// NW(-1,-1 ) N(-1, 0 ) NE(-1, 1 )
//  W( 0,-1 )            E( 0, 1 )
// SW( 1,-1 ) S( 1, 0 ) SE( 1, 1 )
enum { NW, N, NE, E, SE, S, SW, W, CYCL }; // Clockwise
const Coor dR[] = {
//  NW            N             NE            E
    Coor(-1,-1 ), Coor(-1, 0 ), Coor(-1, 1 ), Coor( 0, 1 ),
//  SE            S             SW            W
    Coor( 1, 1 ), Coor( 1, 0 ), Coor( 1,-1 ), Coor( 0,-1 ),
};
// Knight moves
// +---+---+---+---+---+
// |   | 2 |   | 3 |   | -2
// +---+---+---+---+---+
// | 1 |   |   |   | 4 | -1
// +---+---+---+---+---+
// |   |   |   |   |   |  0
// +---+---+---+---+---+
// | 8 |   |   |   | 5 | +1
// +---+---+---+---+---+
// |   | 7 |   | 6 |   | +2
// +---+---+---+---+---+
//  -2  -1   0  +1  +2
const Coor dN[] = {
//  1             2             3             4
    Coor(-1,-2 ), Coor(-2,-1 ), Coor(-2, 1 ), Coor(-1, 2 ),
//  5             6             7             8
    Coor( 1, 2 ), Coor( 2, 1 ), Coor( 2,-1 ), Coor( 1,-2 ),
};
////////////////////////////////////////////////////////
#define offset_color( OFFSET) ( !( ( OFFSET) < 0))
moves_t
Position::KnightMoves( const Figure& fig) const
{
    const Coor& sors{ fig.get_coor()};
    moves_t moves;    
    for( int w{ NW}; w < CYCL; ++w)
    {
        const Coor dest{ sors + dN[w]};
        const int g{ _board.get_offset( dest)};

        if( g == GUARD) continue;
        if( g == EMPTY) {
            //
            moves.emplace_front( sors, dest);

        } else if( offset_color( g) != fig.getColor()) {
            // recapture
            moves.emplace_front( sors, dest, CRON);
        }
    }
    return moves;
}
////////////////////////////////////////////////////////
const UnitMoves Position::_UM[] = {
    &Position::KingMoves,
    &Position::QueenMoves,
    &Position::RookMoves,
    &Position::BishopMoves,
    &Position::KnightMoves,
    &Position::PawnMoves,
};
std::forward_list<Move>
Position::getMoves( const Figure& fig ) const
{
    return (this->*_UM[ fig.getUnit() ])( fig );
}
void Position::spit_moves() const
// Ok loop over active_color figures and dump all
// possible (checked ) moz.
{
    std::stringstream ss;
    int nmov{}; // number of moves( mate)
    for( const auto& fig: _fig[ _active_color]) {
        if( fig.isOutOfPlay()) continue;
        const moves_t& moves{ getMoves( fig)};
        ss << fig, ": ";
        for( const auto& move: moves) {
            if( !ckmov( move)) continue;
            ++nmov;
            ss << move.getDest();
            if( const int p = move.getPmot()) {
                ss << "("
                   << Figure::_UNITS[ _active_color][ p]
                   << ")";
            }
            ss << " ";
        }
        ss << "\n";
    }
    if( !nmov) ss << "Checkmate!!\n";
    std::cout << ss.str();
}
////////////////////////////////////////////////////////
moves_t Position::RangeMoves( const Figure& fig,
                              const int start,
                              const int step) const
{
    moves_t moves;
    // loop over directions
    for( int k{ start}; k < CYCL; k += step)
    {
        const Coor& sors{ fig.get_coor()};
        int g; // destination offset
        Coor dest{ sors};
        while( true)
        {
            dest += dR[k];
            g = _board.get_offset( dest);
            if( g != EMPTY) break;
            moves.emplace_front( sors, dest);
        }
        if( g != GUARD and
            offset_color( g) != fig.getColor()) {
            // capture, re-capture or not?
            moves.emplace_front( sors, dest, CRON);
        }
    }
    return moves;
}
std::forward_list<Move>
Position::RookMoves( const Figure& fig ) const
{
    return RangeMoves( fig, N, 2 );
}
std::forward_list<Move>
Position::BishopMoves( const Figure& fig ) const
{
    return RangeMoves( fig, NW, 2 );
}
std::forward_list<Move>
Position::QueenMoves( const Figure& fig ) const
{
    return RangeMoves( fig, NW, 1 );
}
std::forward_list<Move>
Position::KingMoves( const Figure& fig ) const
{
    moves_t moves{ rytes_.getCastles( *this )};
    const Coor& sors{ fig.get_coor() };
    // loof over World Directions
    for( int k = NW; k < CYCL; ++k )
    {
        const Coor& dest{ sors + dR[k] };
        const int f{ _board.get_offset( dest )};

        if( f == GUARD or undafire( dest )) continue;
        if( f == EMPTY ){
            
            moves.emplace_front( sors, dest );
            
        } else if( offset_color( f ) != _active_color ){
            
            moves.emplace_front( sors, dest, CRON );
        }
    }
    return moves;
}
////////////////////////////////////////////////////////
// promotion blow expansion
// covert to deque, do the job, convert back
void pmot_blow( moves_t& moves )
{
    std::deque<Move> deq( moves.begin(), moves.end() );
    const int n{ static_cast<int>( deq.size() )};

    for( int j = 0; j < n; ++j )
    {
        const Coor&  sors{ deq.front().getSors() };
        const Coor&  dest{ deq.front().getDest() };
        const int8_t type{ deq.front().getType() };
        deq.pop_front();
        
        for( int unit = QUEEN; unit < PAWN; ++unit )
        {
            deq.emplace_back( sors, dest, type | unit );
        }
    }
    moves = moves_t( deq.begin(), deq.end() );
}
moves_t Position::PawnMoves( const Figure& fig) const
{
    // race parameters
    static const int STRT[] = { 3, 8 };
    static const int STOP[] = { 9, 2 };
    // forward direction
    static const int FRWD[] = { S, N };
    // capturings
    static const int LEFT[] = { SE, NW };
    static const int RYTE[] = { SW, NE };
    // shortcuts
    const bool color{ fig.getColor()};
    const Coor& dF{ dR[ FRWD[ color]]};
    const Coor& dL{ dR[ LEFT[ color]]};
    const Coor& dY{ dR[ RYTE[ color]]};
    const Coor& sors{ fig.get_coor()};
    //
    moves_t moves;
    int g; // destination offset
    // fst ck capture /recapture
    for( const Coor& dr: { dL, dY })
    {
        const Coor& dest{ sors + dr };
        g = _board.get_offset( dest);
        if( g == EMPTY) {
            if( dest == _npas) {
                // yef!
                moves.emplace_front( sors, dest, CRON |NPAS);
            }
        } else {
            if( g != GUARD &&
                offset_color( g) != color) {
                moves.emplace_front( sors, dest, CRON);
            }
        }
    }
    // moo forward
    Coor dest{ sors + dF };
    g = _board.get_offset( dest);
    if( g == EMPTY) {
        moves.emplace_front( sors, dest);
        // ck 4 double moov
        if( sors.get_i() == STRT[ color]) {
            dest += dF;
            g = _board.get_offset( dest);
            if( g == EMPTY) {
                moves.emplace_front( sors, dest);
            }
        }
    }
    if( dest.get_i() == STOP[ color]) {
        pmot_blow( moves);
    }        
    return moves;
}
////////////////////////////////////////////////////////
// log:
void Position::makemove( const Move& move )
{
    // forward direction( copy from PawnMoves )
    static const int FRWD[] = { S, N };
    const Coor& dF{ dR[ FRWD[ _active_color ]]};

    const Coor& sors{ move.getSors() };
    const Coor& dest{ move.getDest() };
    
    const int f{ _board.abs_offset( sors )};
    if( move.isCron() ) // or not!
    {
        int g;
        if( move.isNpas() ){
            g = _board.abs_offset( Coor( sors.get_i(),
                                         dest.get_j() ));
            // Debuging for nth time ..
        } else {
            g = _board.abs_offset( dest );
        }
        _fig[ !_active_color ][g].putOutOfPlay();
    }
    _board[ dest ] = _board[ sors ];
    _board[ sors ] = EMPTY;

    _fig[ _active_color ][f].set_coor( dest );
    
    if( move.getPmot() )
    {
        _fig[_active_color ][f].pmot( move.getPmot() );
    }
    if( move.isCast() )
    {
        std::string rsors_cn; // rook source chess notation
        std::string rdest_cn;
        // Now figure the Rook Square.
        // 0 1 2 3 4 5 6 7 8 9 a b
        // ----------- ===========
        if( dest.get_j() < 6 )
        {
            // this have to be changed
            rsors_cn = _active_color ? "a1" : "a8";
            rdest_cn = _active_color ? "d1" : "d8";

        } else
        {
            rsors_cn = _active_color ? "h1" : "h8";
            rdest_cn = _active_color ? "f1" : "f8";
        }
        const Coor rsors( rsors_cn );
        const Coor rdest( rdest_cn );
        
        const int f{ _board.abs_offset( rsors )};

        _board[ rdest ] = _board[ rsors ];
        _board[ rsors ] = EMPTY;
        
        _fig[ _active_color ][f].set_coor( rdest );
    }
    // last but not least check if double pawn moo
    _npas = Coor(); // reset
    if( move.isMove() ){
        if( _fig[ _active_color ][f].getUnit() == PAWN )
        {
            const Coor frwd{ sors + dF };
            if( frwd != dest ) _npas = frwd;
        }
    }
    _active_color = !_active_color;
}
////////////////////////////////////////////////////////
bool Position::undafire( const Coor& coor) const
// Place a virtual figure at coor an ck for the
// corresponding nmy figure.
// Bug #0312: If king is in check from long range piece
// it will shield the square behind the attack, so
// undafire will report it's ok when it's not ok. Zo put
// active side king temporary transparent.
{
    static const Coor* const dW[] = { dR, dN };
    static const int SHORTRANGE[] = { KING, KNIGHT };
    static const int LONGRANGE[] = { BISHOP, ROOK };
    static const int START[] = { NW, N };
    // Backward pawn squares
    static const int BACKWARD[][ RED ] = {
        { NW, NE }, { SW, SE }
    };
    // One idea to change this is to check if coor is
    // empty than enemy is !_active_color otherwise it's
    // the opposite of figure color on that coor.
    const bool nmy{ !_active_color}; // default
    for( int j: { 0, 1 }) // Artifitial Castle
    {
        // Pawn Fork
        const int w{ BACKWARD[ nmy ][j] };
        int f{ _board.get_offset( coor + dR[w] )};   // r
        if( f < EMPTY and offset_color( f ) == nmy ) // e
        {                                            // p
            if( f < 0 ) f = ~f;                      // e
            if( _fig[ nmy ][f].getUnit() == PAWN ){  // t
                return true;                         // e
            }                                        // .
        }
        // King/ Knight
        for( int w{ NW }; w < CYCL; w += 1 )
        {
            int f{ _board.get_offset( coor + dW[j][w] )};
            if( f < EMPTY and offset_color( f ) == nmy )
            {
                if( f < 0 ) f = ~f;
                const int unit{ _fig[ nmy ][f].getUnit() };
                if( unit == SHORTRANGE[j] ) return true;
            }
        }
        // Bishop/ Rook
        for( int w{ START[j] }; w < CYCL; w += 2 )
        {
            Coor r{ coor };
            for( ;; )
            {
                r += dR[w];
                int f{ _board.get_offset( r )};
                if( f == GUARD ) break; // next direction
                if( f == EMPTY ) continue;
                // Transparent King
                if( f == 0 or f == -1 ) continue;
                if( offset_color( f ) == nmy )
                {
                    if( f < 0 ) f = ~f;
                    const int unit{ _fig[ nmy ][f].getUnit() };
                    if( unit == LONGRANGE[j] or unit == QUEEN ){
                        return true;
                    } else {
                        break; // Shield
                    }
                } else {
                    break; // Don't count Röntgen!   
                }
            }
        }
    }
    return false;
}
////////////////////////////////////////////////////////
// log: ? why g1 is undafire ¿                       [v]
std::size_t Position::hash() const
{
    const std::string str(( char* )&_board ,
                          sizeof( _board ));
    
    return std::hash<std::string>{}( str );
}
////////////////////////////////////////////////////////
bool Position::ck() const
{
    const Figure& king{ _fig[ _active_color][ 0]};
    
    return undafire( king.get_coor());
}
////////////////////////////////////////////////////////
std::forward_list<Position> Position::fork() const
// If Bobby is in check thats legal if it's its turn,
// but illegal if it's Anton( Human) turn!! Basically
// don't make any per move checks, make a move and than
// check if your King is in check!?
{
    std::forward_list<Position> list;
    // active_color -> fig -> moves -> makemove -> ck
    for( const auto& fig: _fig[ _active_color]) {
        // bug #0xabcd: figure should be in play
        if( fig.isOutOfPlay()) continue;
        // regardless
        for( const auto& move: getMoves( fig)) {
            Position pos{ *this};// copy
            pos.makemove( move);//
            pos.pass();        // what's going on here?
            if( !pos.ck()) {  // not in check
                pos.pass();  // restore move order
                list.push_front( std::move( pos));
            }
        }
    }
    return list;
}
////////////////////////////////////////////////////////
void Position::dont_spit() const
//
//  rnbqkbnr82
//  pppppppp73
//  ........64
//  ........55
//  ........46
//  ........37
//  PPPPPPPP28
//  RNBQKBNR19
//  abcdefgh
//
{
    std::stringstream ss;
    static const int SANDBOX = BSIZE - FRAMEW;
    for( int i{ FRAMEW }; i < SANDBOX; ++i) {
        for( int j{ FRAMEW }; j < SANDBOX; ++j)
        {
            int f{ _board.offset[i][j]};
            if( f == EMPTY) ss << std::setw( 3) << '.';
            else {
                const bool color{ offset_color( f)};
                if( f < 0) f = ~f;
                const int u{ _fig[ color][ f].getUnit()};
                ss << std::setw( 3), Figure::_UNITS[ color][ u];
            }
        }
        ss << "  " << SANDBOX - i << '\n'; // add width 3
    }
    std::cerr << ss.str()
              << "\n  a  b  c  d  e  f  g  h\n\n";
}
////////////////////////////////////////////////////////
int Position::eval() const
// White minus Black, in-play figures.
{
    // Pawn is 4 units, King's value is irrelevant.
    const static int VALUE[] = {
        0, 36, 20, 14, 12, 4
    };
    int sm[] = { 0, 0 };
    for( const bool color: { BLACK, WHITE }) {
        for( const auto& fig: _fig[ color]) {
            if( fig.isOutOfPlay()) continue;
            sm[ color] += VALUE[ fig.getUnit()];
        }
    }
    return sm[ WHITE] - sm[ BLACK];
}
////////////////////////////////////////////////////////
// make it happen
bool __lt__( const Score& p, const Score& q)
{
    return ( p.eval < q.eval); // Why I have to put
    // parenthesis here?
}
bool __gt__( const Score& p, const Score& q)
{
    return p.eval > q.eval; // Thats why!
}
// let't try typedef alone this time
typedef bool ( *cooperator_t)( const Score&, const Score&);
// Comparator didn't pass the spell check test!!
std::vector<Score>
Position::getScores( const int depth) const
// Khm-khm .. Now the heard of the program starts around
// here, coz we are approaching the "minimax" algorithm,
// originally developed by non other else than John von
// Neumann. Make a loop over possible moves, put them
// into vector, make a copy of the current position and
// apply every move on it, evaluate the resulting positi-
// ons using flip flop ( minimax) routines and sort them
// in decreasing order. Next having ordered moves we can
// play a move depending on the difficulty level etc.
{
    // vector cos of sorting
    std::vector<Score> vec;
    // make a loop over active color figures
    for( const auto& fig: _fig[ _active_color]) {
        // skip out of play units
        if( fig.isOutOfPlay()) continue;
        // loop over all possible moves
        for( const auto& move: getMoves( fig)) {
            // make a copy to the current position
            Position copy{ *this};
            // apply the move, this will change the
            // active side as well
            copy.makemove( move);
            // ck vhether King iz'n check
            copy.pass();
            if( copy.ck()) continue; // yep
            copy.pass();
            // depth evaluate the position and push evaluation
            // to vec alongside with the corresponding move
            Score score{ move, copy.flipflop( depth - 1)};
            vec.push_back( score);
        }
    }
    // I had a problem here, cos Move fields were
    // declared const, and the compiler successfully
    // cover that fix from me. Thanks God Stack
    // Overflow exist.
    cooperator_t coop = _active_color ? __gt__ : __lt__;
    std::sort( vec.begin(), vec.end(), coop);

    return vec;
}
////////////////////////////////////////////////////////
int flop( const Position& p, const int depth);
//
int flip( const Position& p, const int depth)
// Black
{
    if( !depth) return p.eval(); // regardless
    int minscore{ INT_MAX };
    for( const auto& q: p.fork())
    {
        int score;
        score = flop( q, depth - 1);
        if( score < minscore) minscore = score;
    }
    return minscore;
}
int flop( const Position& p, const int depth)
// White
{
    if( !depth ) return p.eval();
    int maxscore{ INT_MIN };
    for( const auto& q: p.fork())
    {
        const int score{ flip( q, depth - 1)};
        if( maxscore < score ) maxscore = score;
    }
    return maxscore;
}
int Position::flipflop( const int depth) const
// depth evaluation
{
    if( _active_color) return flop( *this, depth);

    return flip( *this, depth);
}
////////////////////////////////////////////////////////
bool Position::ckmov( const Move& move) const
// After every move it will be illegal if the King is in
// check, so check for that.
{
    Position copy{ *this};
    copy.makemove( move);
    copy.pass();
    return !copy.ck();
}
////////////////////////////////////////////////////////
void Position::debug()
{
}
////////////////////////////////////////////////////////
// log: - Makefile, dependencies                     []
// - rook moves and castle rights                    []
// - ckmov                                           []
// - sort figures after fen initialization in
// decreasing order and start geting moves from
// behind                                            []
