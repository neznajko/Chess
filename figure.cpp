                          ////
////////////////////////////////////////////////////////
# include "node.hpp"
# include <cctype> // isupper, tolower
# include <sstream>
////////////////////////////////////////////////////////
const std::string Figure::_TRAITS[] = {
    "kqrbnp", // Black
    "KQRBNP", // White
};
// Pawn is 4 units, King's value should be irrelevant,
// but is possible to make an implementation where the
// King is like a common figure.
const int Figure::_VALUES[] = {
    800, 36, 20, 14, 12, 4,
};
const int Figure::_AGILITY[] = {
    0, 1, 2, 4, 5, 3,
};
const std::string Figure::_NAMES[] = {
    "King", "Queen", "Rook", "Bishop", "Knight", "Pawn"
};
const std::string Figure::_PIX[][ NOFIGS] = {
    { "[±]", "[ ]", ",'`", "(†)", " ‡ ", " ø " },
    { "(±)", "( )", ",'`", "(†)", " ‡ ", " o " },
};
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& ostrm, const Figure& f)
{
    const int type{ f.type()};
    if( type != Figure::PAWN) {
        ostrm << f.traits();
    }
    return( ostrm << f.coor());
}
bool operator<( const Figure& f, const Figure& g)
{
    return( f.agility() < g.agility());
}
Figure* Figure::factory( const char c,
                         const Coor& coor)
{
    const bool color = isupper( c);
    switch( tolower( c)) {
        case 'k': return new King( coor, color);
        case 'q': return new Queen( coor, color);
        case 'r': return new Rook( coor, color);
        case 'b': return new Bishop( coor, color);
        case 'n': return new Knight( coor, color);
        case 'p': return new Pawn( coor, color);
        default: break;    
    }
    return nullptr;
}
std::string Figure::mozstr() const
{
    std::stringstream strm;
    if( moz.empty()) return "";
    strm << pix() << _coor.chessnot();
    for( const auto& mov: moz) {
        strm << mov.str();
    }
    strm << endl;
    return strm.str();
}
void Figure::ranger_moz( const int start, const int step)
{
    for( int k{ start}; k < Coor::WORLD; k += step) {
        Coor dest{ _coor};
        Unit* u;
        while( true) {
            dest += Coor::dR[k];
            u = brd_->getUnit( dest);
            if( u != Unit::ZILCH) break;
            moz.emplace_back( dest);
        }
        if( u != Unit::GUARD and
            _color != u->f->color()) {
            // capture, re-capture or not?
            moz.emplace_back( dest, CRON);
        }
    }
}
void Figure::setNode( Node* node) {
    _node = node;
    brd_ = &( _node->board);
}
////////////////////////////////////////////////////////
void Knight::getmoz()
{
    for( int j{ Coor::NW}; j < Coor::WORLD; j++) {
        const Coor& r{ _coor + Coor::dN[ j]};
        Unit* u{ brd_->getUnit( r)};
        if( u == Unit::GUARD) continue;
        if( u == Unit::ZILCH) {
            moz.emplace_back( r);
        } else if( u->f->color() != _color) {
            moz.emplace_back( r, CRON);
        }
    }
}
void Bishop::getmoz()
{
    ranger_moz( Coor::NW, 2);
}
void Rook::getmoz()
{
    ranger_moz( Coor::N, 2);
}
void Queen::getmoz()
{
    ranger_moz( Coor::NW, 1);
}
////////////////////////////////////////////////////////
//   0 1 2 3 4 5 6 7 8 9 a b
// 0 - - - - - - - - - - - -
// 1 - - - - - - - - - - - -   NW N NE
// 2 - - r n b q k b n r - -    W *  E
// 3 - - p p p p p p p p - -   SW S SE
// 4 - - . . . . . . . . - -
// 5 - - . . . . . . . . - -
// 6 - - . . . . . . . . - -
// 7 - - . . . . . . . . - -
// 8 - - P P P P P P P P - -
// 9 - - R N B Q K B N R - -
// a - - - - - - - - - - - -
// b - - - - - - - - - - - -
void Pawn::getmoz()
{
    // Pawn race parameters B/W
    static const int STAT[] = { 3, 8 };
    static const int STAP[] = { STAT[ WHITE] + 1,
                                STAT[ BLACK] - 1 };
    // Forward directions
    static const int FRWD[] = { Coor::S, Coor::N };
    // Capturings
    static const int LEFT[] = { Coor::SE, Coor::NW };
    static const int RYTE[] = { Coor::SW, Coor::NE };
    // shortcuts
    const Coor& dF{ Coor::dR[ FRWD[ _color]]};
    const Coor& dL{ Coor::dR[ LEFT[ _color]]};
    const Coor& dY{ Coor::dR[ RYTE[ _color]]};
    // fst ck capture /re-capture //////////////////////////////
    for( const Coor& dr: { dL, dY }) {
        const Coor& dest{ _coor + dr};
        Unit* u{ brd_->getUnit( dest)};
        if( u == Unit::ZILCH) {
            if( dest == _node->npas) {
                moz.emplace_back( dest, CRON | NPAS);
            }
        } else {
            if( u != Unit::GUARD && u->f->color() != _color) {
                moz.emplace_back( dest, CRON);
            }
        }
    }
    // mov forward
    Coor&& dest{ _coor + dF};
    Unit* u{ brd_->getUnit( dest)};
    if( u == Unit::ZILCH) {
        moz.emplace_back( dest);
        // ck for dble mov
        if( _coor.i == STAT[ _color]) {
            dest += dF;
            u = brd_->getUnit( dest);
            if( u == Unit::ZILCH) {
                moz.emplace_back( dest);
            }
        }
    }
    if( dest.i == STAP[ _color]) {
        pmot_blow();
    }        
}
////////////////////////////////////////////////////////
void Pawn::pmot_blow() {
    int n = moz.size();
    for( int j = 0; j < n; ++j) {
        const Coor&   dest{ moz.front().dest};
        const uint8_t type{ moz.front().type};
        moz.pop_front();
        int unit = Figure::KING;
        while( ++unit != Figure::PAWN) {
            moz.emplace_back( dest, type | unit);
        }
    }
}
////////////////////////////////////////////////////////
void King::getmoz() {
    std::cout << "King moz: ";
    std::cout << undafire() << endl;
    // .. Consider passing node instead _brd pointer
    // work out move unmove stof and for every possible
    // king move chck if it's undafire if not add to moz
    std::cout << "Checking aura\n";
    chck_aura();
    for( int j = Coor::NW; j <= Coor::WORLD; j++) {
        std::cout << j << " " << aura[ j] << endl;
    }
}
////////////////////////////////////////////////////////
// For Kings and Knights 
bool King::shortrange( const Coor* const dr,
                       const int type) const {
    // loop over World directions
    for( int w = Coor::NW; w < Coor::WORLD; w++) {
        // get corresponding square
        const Coor& coor = _coor + dr[ w];
        // Skip empty or guard squares
        Unit* u = brd_->getUnit( coor);
        if( u == Unit::ZILCH or u == Unit::GUARD) {
            continue;
        }
        // Chck if it's enemy type
        if( u->f->type()  == type and
            u->f->color() != _color) {
            return true; // vhether it's mate is unclear
        }
    }
    return false;
}
// For Rooks Bishops and Queens
bool King::longrange( const int start,
                      const int type) const {
    // loop over World directions
    for( int w = start; w < Coor::WORLD; w += 2) {
        const Coor& dr = Coor::dR[ w];
        Coor coor{ _coor};
        for(;; coor += dr) {
            const Unit* const u{ brd_->getUnit( coor)};
            if( u == Unit::GUARD) break;
            if( u == Unit::ZILCH) continue;
            // Chck if it's enemy type
            if( u->f->color() != _color and
                (u->f->type() == type or
                 u->f->type() == Figure::QUEEN)) {
                return true; // vhether it's mate it's unclear
            }   
        }
    }
    return false;
}
bool King::poke() {
    // Basically check in the forking directions vhether there
    // is an enemy pawn.
    static const int FORKS = 2;
    static const int F[][ FORKS] = {
        { Coor::SW, Coor::SE },
        { Coor::NW, Coor::NE }
    };
    for( int j = 0; j < FORKS; j++) {
        const int f = F[ _color][ j]; // forking direction
        const Coor& coor = _coor + Coor::dR[ f];
        // get the unit at that square and make the chck
        const Unit* const u{ brd_->getUnit( coor)};
        if( u == Unit::ZILCH or u == Unit::GUARD) {
            continue;
        }
        if( u->f->color() != _color and
            u->f->type() == Figure::PAWN) {
            return true;
        }
    }
    return false;
}
bool King::undafire() {
    // fst check Shortrange pieces Knight and King
    if( shortrange( Coor::dR, Figure::KING)) return true;
    // chck if enemy has any Knights, bfor calling shortrange
    if( _node->army[!_color].cntrs[ Figure::KNIGHT] > 0) {
        if( shortrange( Coor::dN, Figure::KNIGHT)) {
            return true;
        }
    }
    // next chck for Longrange Units
    if( _node->army[!_color].cntrs[ Figure::BISHOP] > 0 or
        _node->army[!_color].cntrs[ Figure::QUEEN] > 0) {
        if( longrange( Coor::NW, Figure::BISHOP)) {
            return true;
        }
    }
    if( _node->army[!_color].cntrs[ Figure::ROOK] > 0  or
        _node->army[!_color].cntrs[ Figure::QUEEN] > 0) {
        if( longrange( Coor::N, Figure::ROOK)) {
            return true;
        }
    }
    // No-pawn endgames are exceptions, so don't check
    // enemy counters.
    return poke(); // Ok
}
////////////////////////////////////////////////////////
void King::chck_aura() {
    // Clear current board square, and obtaing King's
    // Unit pointer. There are some complains about
    // ZILCH being const, so use nullptr instead.
    Unit* k = brd_->setUnit( _coor, nullptr);
    // loop over all directions including self position
    for( int j = Coor::NW; j <= Coor::WORLD; j++) {
        _coor += Coor::dR[ j];
        const Unit* const u = brd_->getUnit( _coor);
        if( u == Unit::GUARD or  // out of dojo or
            u != Unit::ZILCH and // frendly figure
            u->f->color() == _color) { 
            aura[ j] = false;
        } else {
            // - make Board::setUnit( coor)
            // - backup destination unit
            // - move king into that position
            // - call undafire
            // - re-establish
            Unit* p = brd_->setUnit( _coor, k);
            aura[ j] = !undafire();
            brd_->setUnit( _coor, p);
        }
        _coor -= Coor::dR[ j];
    }
}
////////////////////////////////////////////////////////
const Unit* Unit::ZILCH = nullptr;
const Unit* Unit::GUARD = Unit::ZILCH + 1;
////////////////////////////////////////////////////////
// log:
////////////////////////////////////////////////////////
    ////                                        ////
