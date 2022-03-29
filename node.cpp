                          ////
////////////////////////////////////////////////////////
# include "node.hpp"
# include <sstream>
////////////////////////////////////////////////////////
////////////////////////////////////////////////// board
Board::Board() // make it simple
{
    for( int i{}; i < BOARDZE; ++i) {
        for( int j{}; j < BOARDZE; ++j) {
            u[ i][ j] = ( Unit*) Unit::GUARD;
        }
    }
    for( int i{ DOJOBEGIN}; i < DOJOEND; ++i) {
        for( int j{ DOJOBEGIN}; j < DOJOEND; ++j) {
            u[ i][ j] = ( Unit*) Unit::ZILCH;
        }
    }    
}
std::string Board::str() const
{
    static const std::size_t N{ sizeof( Board)};
    return std::string(( char*) this, N);
}
// replace with ::v and return the previous unit
Unit* Board::setUnit( const Coor& coor, Unit* v) {
    auto [ i, j] = coor; // deconstruct
    Unit* p = u[ i][ j]; // backup
    u[ i][ j] = v;       // replace
    return p;            // return
}
/////////////////////>>/////////////////////////////////
// , . k r r . , . //                           { var a}
// p b p , . , p p //                           
// , p , . , n , . //
// . , . , p , . , //
// , . , . P . , . //
// . , . P . R . , //
// P P P . , . , P //
// . , B , . R K , //
/////////////////////--/////////////////////////////////
// ,__.__,__.__,__.__,__.__,                    { var b}
// |__¦k_|r_¦__|r_¦__|__¦__|8 
// ¦p_|p_¦__|__¦p_|q_¦p_|__¦7
// |__¦__|__¦__|__¦p_|__¦__|6
// ¦__|__¦p_|__¦__|__¦__|__¦5
// |__¦__|__¦_P|__¦__|__¦__|4
// ¦__|_Q¦__|__¦_P|__¦__|__¦3
// |__¦__|_P¦__|__¦_P|_P¦_P|2
// ¦_R|__¦__|__¦_R|_K¦__|__¦1
//   a  b  c  d  e  f  g  h
////////////////////////////////////////////////////////
// r n b q k b n r                              { thats}
// p p p p p p p p
// . . . . . . . .
// . . . . . . . .
// . . . . . . . .
// . . . . . . . .
// P P P P P P P P
// R N B Q K B N R
////////////////////////////////////////////////////////
// There is no color info here so it can be redirected
// to log file for debugging.
std::ostream&
operator<<( std::ostream& ostrm, const Board& board)
{
    for( int i{ DOJOBEGIN}; i < DOJOEND; ++i) {
        for( int j{ DOJOBEGIN}; j < DOJOEND; ++j) {
            Unit* u{ board.u[ i][ j]};
            if( u == Unit::ZILCH) {
                ostrm << " .";
            } else {
                ostrm << ' ' << u->f->traits();
            }
        }
        ostrm << endl;
    }
    return ostrm;
}
////////////////////////////////////////////////////////
const std::string Army::NAME[] = { "Black", "White"};
Unit* Army::insert( Figure* f)
{
    if( f->type() == Figure::KING) {
        king->f = f;
        return king;
    }
    // Insert f in agility decreasing order.
    Unit* p = king; // find[ p]revious node
    // The king might be not set so make sure p->next->f
    // is not nullptr.
    while( p->next->f and
           f->agility() < p->next->f->agility()) {
        p = p->next;
    } // [ ok] now insert the coin
    Unit* u = new Unit( f);
    u->next = p->next;
    p->next = u;
    u->next->prev = u;
    u->prev = p;
    return u;
}
Army::~Army()
{
    while( king->next != king) {
        unlink( king->next, true);
    }
    delete king;
    for( Unit* p: bench) {
        delete p;
    }
}
void Army::unlink( Unit* p, const bool del)
{
    p->prev->next = p->next;
    p->next->prev = p->prev;
    if( del) {
        delete p;
    } else {
        bench.push_back( p);
    }
}
void Army::getmoz()
{
# ifdef DEBUG
    if(! king->f) return;
# endif
    Unit* u{ king};
    do {
        u = u->next;
        u->f->getmoz();
    } while( u != king);
}
void Army::dmpmoz()
{
# ifdef DEBUG
    if(! king->f) return;
# endif
    Unit* u{ king};
    do {
        u = u->next;
        std::cout << u->f->mozstr();
    } while( u != king);
}
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& ostrm, const Army& army)
{
    Unit* u{ army.king};
    do {
        u = u->next;
        if( u->f) {
            ostrm << *( u->f) << " ";
        } else {
            ostrm << "VOID";
        }
    } while( u != army.king);
    return ostrm;
}
////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////
// Forsyth–Edwards Notation
// After 1.e4 c5:
// rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR
// w KQkq c6 0 2
Node::Node( const std::string& fen)
{
    int i{ FRAMEWID};
    int j{ i};
    std::vector<std::string> vec{ split( fen)};
    for( const char c: vec[ 0]) {
        if( c == '/') { // new row
            j = FRAMEWID; // reset column
            ++i;          // increment row
        } else if( isdigit( c)) { // empty sqares
            j += c - '0'; // rewind
        } else {
            Figure* f{ Figure::factory( c, Coor( i, j))};
            army[ f->color()].cntrs[ f->type()]++;
            f->setNode( this);
            board.u[ i][ j] = army[ f->color()].insert( f);
            j++;
        }
    }
    // Get the other stuff from FEN.
    active_color = vec[ 1].front() == 'w';
    // rytes_.setFenRytes( vec[ 2]);
    // 0h! pä 'sø
    if( vec[ 3] != "-") npas = Coor( vec[ 3]);
}
////////////////////////////////////////////////////////
// log:
////////////////////////////////////////////////////////
    ////                                        ////   
