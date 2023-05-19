////////////////////////////////////////////////////////
# include <iostream>
# include <vector>
////////////////////////////////////////////////////////
template <typename T>
std::ostream& operator<<( std::ostream& ostrm,
                          const std::vector<T>& vec ){
    ostrm << "[";
    for( std::size_t j{}, n{ vec.size()}; j < n; ++j ){
        ostrm << " " << vec[ j ] << " ,"[ j < n - 1 ];
    }
    return ostrm << "]";
}
////////////////////////////////////////////////////////
# include <chrono>
////////////////////////////////////////////////////////
void benchmark( void (*f)(), int NfLoops ){
    using namespace std::chrono;
    const auto start{ steady_clock::now()};
    while( NfLoops-- > 0 ){
        f();
    }
    const auto stop{ steady_clock::now()};
    duration<double> total{ stop - start };
    std::cout << total.count() << " sec\n";
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////[SQ]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
# include <unordered_set>
////////////////////////////////////////////////////////
class Unit;
class Generator;
////////////////////////////////////////////////////////
enum { BLACK, WHITE, RED, BLUE };
////////////////////////////////////////////////////////
typedef char offset_t;
typedef int color_t;
////////////////////////////////////////////////////////
class SQ {
    const offset_t offset;
    Unit * unit;
    std::unordered_set<Generator *> subscribers;
    void Notify() const;
public:
    SQ( const offset_t offset, Unit * unit ):
        offset( offset ),
        unit( unit )
        {}
    void Register( Generator * const observer ){
        subscribers.insert( observer );
    }
    void Unregister( Generator * const observer ){
        subscribers.erase( observer );
    }
    Unit const * GetUnit() const {
        return unit;
    }
    Unit * SetUnit( Unit * const nextUnit );
    bool Check( const color_t color );
    friend std::ostream&
    operator<<( std::ostream& os, const SQ& sq );
};
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
/////////////////////////////////////////////////[Board]
//    | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
//    +---+---+---+---+---+---+---+---+---+---+
//  0 |   | R | N | B | Q | K | B | N | R |   |
//    +---+---+---+---+---+---+---+---+---+---+
//  1 |   |   |   |   |   |   |   |   |   |   |
//    +---+---+---+---+---+---+---+---+---+---+
//  2 | 8 | 21| 22| 23| 24| 25| 26| 27| 28|   |
//    +---+---+---+---+---+---+---+---+---+---+
//  3 | 7 | 31| 32| 33| 34| 35| 36| 37| 38|   |
//    +---+---+---+---+---+---+---+---+---+---+
//  4 | 6 | 41| 42| 43| 44| 45| 46| 47| 48|   |
//    +---+---+---+---+---+---+---+---+---+---+
//  5 | 5 | 51| 52| 53| 54| 55| 56| 57| 58|   |
//    +---+---+---+---+---+---+---+---+---+---+
//  6 | 4 | 61| 62| 63| 64| 65| 66| 67| 68|   |
//    +---+---+---+---+---+---+---+---+---+---+
//  7 | 3 | 71| 72| 73| 74| 75| 76| 77| 78|   |
//    +---+---+---+---+---+---+---+---+---+---+
//  8 | 2 | 81| 82| 83| 84| 85| 86| 87| 88|   |
//    +---+---+---+---+---+---+---+---+---+---+
//  9 | 1 | 91| 92| 93| 94| 95| 96| 97| 98|   |
//    +---+---+---+---+---+---+---+---+---+---+
// 10 |   | A | B | C | D | E | F | G | H |   |
//    +---+---+---+---+---+---+---+---+---+---+
// 11 |   |   |   |   |   |   |   |   |   |   |
//    +---+---+---+---+---+---+---+---+---+---+
////////////////////////////////////////////////////////
# include <string>
# include <cctype>
////////////////////////////////////////////////////////
enum { QSIDE, KSIDE, CENTER };
////////////////////////////////////////////////////////
typedef bool flank_t;
////////////////////////////////////////////////////////
namespace Casl {
    static constexpr int ROOK_COL[]{ 1, 8 };
    static constexpr int KING_DR[]{ -1, 1 };
    static constexpr offset_t KING_FROM[]{ // B, W
        25, 95
    }; //                                Q K
    static constexpr offset_t KING_TO[][ CENTER ]{
        { 23, 27 }, { 93, 97 }
    };
    static constexpr offset_t ROOK_FROM[][ CENTER ]{
        { 21, 28 }, { 91, 98 }
    };
    static constexpr offset_t ROOK_TO[][ CENTER ]{
        { 24, 26 }, { 94, 96 }
    };
    ////////////////////////////////////////////////////
    // B = 0, Q = 0 0 - 00 BQ C = {B,W}
    // W = 1, K = 1 1 - 01 BK F = {Q,K}
    //              2 - 10 WQ J = {0,1,2,3}
    //              3 - 11 WK J = C << 1 + F
    // M = { 1000, 0100, 0010, 0001 }, M = 1 << J
    ///////////////////////////////////////////////////=
    // FEN + CF + J + MASK     8 4 2 1
    //  k  - BK - 1 -  2       K Q k q
    //  q  - BQ - 0 -  1
    //  K  - WK - 3 -  8
    //  Q  - WQ - 2 -  4
    ////////////////////////////////////////////////////
    color_t GetColor( const char c ){
        return std::isupper( c ) != 0;
    }
    flank_t GetFlank( const char c ){
        return std::tolower( c ) == 'k'; 
    }
    int GetJ( const color_t c, const flank_t f ){
        return ( c << 1 ) + f;
    }
    color_t GetC( const int j ){
        return j >> 1;
    }
    flank_t GetF( const int j ){
        return j & 1;
    }
    int GetMask( color_t c, flank_t f ){
        return 1 << GetJ( c, f );
    }
    int GetMask( const char c ){
        return GetMask( GetColor( c ), GetFlank( c ));
    }
    int GetRytes( const std::string& fen ){
        int rytes{};
        if( fen != "-" ){
            for( const char c: fen ){
                rytes |= GetMask( c );
            }
        }
        return rytes;
    }
    std::string GetFEN( int rytes ){
        static const char C[]{ "qkQK" };
        std::string fen;
        for( int j{}; rytes; rytes >>= 1, ++j ){
            if( rytes & 1 ){
                fen.push_back( C[ j ]);
            }
        }
        return fen.empty() ? "-" : fen;
    }
};
////////////////////////////////////////////////////////
class Board {
    std::vector<SQ> sqs;
public:
    static constexpr int WIDTH{ 10 };
    static constexpr int HEIGHT{ 12 };
    static constexpr int SIZE{ WIDTH * HEIGHT };
    static constexpr int FRAME_WIDTH{ 1 };
    static constexpr int FRAME_HEIGHT{ 2 };
    static constexpr int PMOT_ROW[]{ 9, 2 };
    static constexpr int PAWN_ROW[]{ 3, 8 };
    static offset_t GetOffset( const int row,
                               const int col ){
        return row * WIDTH + col;
    }
    static offset_t GetOffset( const std::string& sqr );
    static int GetRow( const offset_t offset ){
        return offset/ WIDTH;
    }
    static int GetCol( const offset_t offset ){
        return offset % WIDTH;
    }
    static char GetFile( const offset_t offset ){
        return "!abcdefgh?"[ GetCol( offset )];
    }
    static char GetRank( const offset_t offset ){
        return "!!87654321??"[ GetRow( offset )];
    }
    static std::string GetCoord( const offset_t offset ){
        static std::string coord{ "g6" };
        coord[ 0 ] = GetFile( offset );
        coord[ 1 ] = GetRank( offset );
        return coord;
    }
    Board();
    Unit * Land( Unit * const unit, const offset_t k );
    Unit * LiftOff( const offset_t k );
    Unit * Travel( const offset_t src,
                   const offset_t dst );
    void Register( Generator * const gen,
                   const offset_t offset );
    void Unregister( Generator * const gen,
                     const offset_t offset );
    void InLoopUnregister( Generator * const gen,
                           const offset_t offset );
    Unit const * const GetUnit( const offset_t k ) const
        {
            return sqs[ k ].GetUnit();
        }
    color_t GetUnitColor( const offset_t k ) const;
    bool Check( const offset_t k, const color_t color ){
        return sqs[ k ].Check( color );
    }
    friend class Painter;
    friend std::ostream&
    operator<<( std::ostream& os, const Board& board );
};
////////////////////////////////////////////////////////
//////////////////////////////////////////////////[Unit]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
enum { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, JOKER };
////////////////////////////////////////////////////////
typedef unsigned char fig_t;
////////////////////////////////////////////////////////
class Node;
class Generator;
////////////////////////////////////////////////////////
class Unit {
    const color_t color;
    fig_t fig; // can change via promotn
    offset_t offset{};
public:
    static Unit * const NIL; // empty square
    static Unit * const NOP; // guard square
    Unit( const fig_t fig,
          const color_t color,
          Node * const node ):
        color( color ),
        node( node )
        {
            SetFig( fig );
        }
    Node * const node; // backdoor pointer
    Generator * gen{}; // changing under promotn
    Unit * prev;
    Unit * next;
    bool isNIL() const {
        return this == NIL;
    }
    bool isNOP() const {
        return this == NOP;
    }
    color_t GetColor() const {
        return color;
    }
    fig_t GetFig() const {
        return fig;
    }
    offset_t GetOffset() const {
        return offset;
    }
    void SetFig( const fig_t fig );
    std::string GetStr() const;
    friend SQ;
    friend std::ostream&
    operator<<( std::ostream& os,
                Unit const * const unit );
};
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& os, Unit const * const unit ){
    return os << unit->GetStr()
              << Board::GetCoord( unit->offset );
}
////////////////////////////////////////////////////////
/////////////////////////////////////////////[Generator]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
struct Move;
////////////////////////////////////////////////////////
class Generator {
protected:
    static constexpr int NF_DR{ 8 };
    static constexpr offset_t
    SHORTRANGE_DR[][ NF_DR ]{ // King
        { -Board::WIDTH - 1, // NW
          -Board::WIDTH,     // N
          -Board::WIDTH + 1, // NE     0 1 2
          +1,                //  E     7 k 3
          +Board::WIDTH + 1, // SE     6 5 4
          +Board::WIDTH,     // S
          +Board::WIDTH - 1, // SW
          -1,                //  W
        }, // Knight
        { -Board::WIDTH * 2 - 1, // 
          -Board::WIDTH * 2 + 1, //     0 1
          -Board::WIDTH * 1 + 2, //    7   2
          +Board::WIDTH * 1 + 2, //      n
          +Board::WIDTH * 2 + 1, //    6   3
          +Board::WIDTH * 2 - 1, //     5 4
          +Board::WIDTH * 1 - 2, // 
          -Board::WIDTH * 1 - 2, // 
        }
    };
    Node * const node;
    Board& board;
    std::unordered_set<offset_t> offsets;
    static int Cmp( const int a, const int b ){
        return a > b ? 0 : a == b ? 1 : 2;
    }
    static int GetDirection( const offset_t a,
                             const offset_t b ){
        static const int COMPASS[][3]{
            { 0, 1, 2 },
            { 7, 8, 3 },
            { 6, 5, 4 },
        };
        const int i{ Cmp( Board::GetRow( a ),
                          Board::GetRow( b ))};
        const int j{ Cmp( Board::GetCol( a ),
                          Board::GetCol( b ))};
        return COMPASS[ i ][ j ];
    }
    ///////////////////////////////////////////////////=
public:
    Unit * const unit;
    static Generator * Factory( Unit * const unit );
    Generator( Unit * const unit );
    virtual ~Generator() {}
    virtual void Subscribe() = 0;
    void Unsubscribe();
    virtual void Update( const offset_t offset ) = 0;
    virtual void GetMoves
    ( std::vector<Move>& movs ) const;
    virtual std::string GetStr() const;
    void Insert( const offset_t k ){
        offsets.insert( k );
    }
    void Erase( const offset_t k ){
        offsets.erase( k );
    }
};
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& os,
            Generator const * const gen );
////////////////////////////////////////////////////////
/////////////////////////////////////////[ShortRangeGen]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
template <fig_t FIG>
class ShortRangeGen: public Generator {
    static constexpr offset_t const * const DR{
        Generator::SHORTRANGE_DR[ FIG == KNIGHT ]
    };
public:
    ShortRangeGen( Unit * const unit ):
        Generator( unit ){}
    void Subscribe();
    void Update( const offset_t offset ){}
};
////////////////////////////////////////////////////////
//////////////////////////////////////////[LongRangeGen]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
typedef void (Board::*RegisterPtr)( Generator * const,
                                    const offset_t );
////////////////////////////////////////////////////////
template <fig_t FIG>
class LongRangeGen: public Generator {
    static constexpr int LONGRANGE_START[]{
        -1, 0, 1, 0
    };
    static constexpr int LONGRANGE_STEP[]{
        -1, 1, 2, 2
    };
    static constexpr offset_t const * const DR{
        Generator::SHORTRANGE_DR[ KING ]
    };
    static constexpr int START{ LONGRANGE_START[ FIG ]};
    static constexpr int STEP{  LONGRANGE_STEP[  FIG ]};
public:
    LongRangeGen( Unit * const unit ):
        Generator( unit ){}
    void Subscribe();
    void Update( const offset_t offset );
    void Scan( RegisterPtr f, int b, int dr );
};
////////////////////////////////////////////////////////
///////////////////////////////////////////////[CaslGen]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
class CaslGen: public Generator {
    const color_t color;
    const int kingDr;
    const offset_t kingSrc;
    const offset_t kingDst;
    const offset_t rookSrc;
    const int mask;
    int figuresCounter{};
public:
    CaslGen( Unit * const king, const flank_t flank ):
        Generator( king ),
        color( king->GetColor()),
        kingDr( Casl::KING_DR[ flank ]),
        kingSrc( Casl::KING_FROM[ color ]),
        kingDst( Casl::KING_TO[ color ][ flank ]),
        rookSrc( Casl::ROOK_FROM[ color ][ flank ]),
        mask( Casl::GetMask( color, flank ))
        {}
    void FlipFlop();
    void Subscribe();
    void Update( const offset_t offset );
    std::string GetStr() const override;
    bool Go() const;
};
////////////////////////////////////////////////////////
///////////////////////////////////////////////[KingGen]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
class KingGen: public ShortRangeGen<KING> {
public:
    CaslGen * const casl[ CENTER ];
    KingGen( Unit * const unit ):
        ShortRangeGen<KING>( unit ),
        casl{ new CaslGen( unit, QSIDE ),
              new CaslGen( unit, KSIDE )}
        {}
    ~KingGen() {
        for( const auto flank: { QSIDE, KSIDE }){
            delete casl[ flank ];
        }
    }
    void GetMoves( std::vector<Move>& movs )
        const override;
    std::string GetStr() const override;
    friend class Node;
};
///////////////////////////////////////////////[PawnGen]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
template <color_t C>
class PawnGen: public Generator {
    static const int FORWARD{ 
        C ? -Board::WIDTH : Board::WIDTH
    };
    static const int PMOT_ROW{ Board::PMOT_ROW[ C ]};
    static const int PAWN_ROW{ Board::PAWN_ROW[ C ]};
public:
    PawnGen( Unit * const unit ):
        Generator( unit ){}
    void Subscribe();
    void Update( const offset_t offset ){}
    void GetMoves( std::vector<Move>& movs )
        const override;
};
////////////////////////////////////////////////////////
//////////////////////////////////////////////////[Army]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
class Army { // dll( ring)
public:
    Unit * const king;
    std::vector<Unit *> bench; // out of play units
    Army( Unit * const king ): king( king ){
        king->prev = king->next = king;
    }
    ~Army();
    Unit * insert( const fig_t type,
                   const color_t color,
                   Node * const node );
    void unlink( Unit * const p, const bool del=false );
    Unit * dance();
};
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& ostrm, const Army& army ){
    static const std::string name[]{
        "Black", "White"
    };
    ostrm << "<" << name[ army.king->GetColor()]
          << ">\n";
    Unit * unit{ army.king };
    do {
        ostrm << unit->gen;
        unit = unit->next;
    } while( unit != army.king );
    ostrm << army.bench << std::endl;
    return ostrm;
}
////////////////////////////////////////////////////////
# include <map>
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//////////////////////////////////////////////////[Node]
////////////////////////////////////////////////////////
typedef unsigned long long u_64;
typedef unsigned char rytes_t;
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
};
////////////////////////////////////////////////////////
//////////////////////////////////////////////////[Move]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
// +---+---+---+---+---+---+---+---+
// | P | L | S | N | B | R | Q | C |
// +---+---+---+---+---+---+---+---+
// C - Capture Recapture Or Not, if this bit is set it's
// capture or recapture, otherwise it's not.
// Q,R,B,N - Queen, Rook, Bishop, kNight promotion.
// S,L - Short and Long Castles.
// P - capture òn pá só
////////////////////////////////////////////////////////
typedef unsigned char move_t;
////////////////////////////////////////////////////////
constexpr move_t MOVE{   0           };
constexpr move_t CRON{        1 << 0 };
constexpr move_t QUEEN_PMOT{  1 << 1 };
constexpr move_t ROOK__PMOT{  1 << 2 };
constexpr move_t BSHOP_PMOT{  1 << 3 };
constexpr move_t NIGHT_PMOT{  1 << 4 };
constexpr move_t SHOTCASL{    1 << 5 };
constexpr move_t LONGCASL{    1 << 6 };
constexpr move_t NPAS{        1 << 7 };
struct Move { // Define no constructors here
    move_t type; 
    offset_t src;  
    offset_t dst;  
    offset_t enPassant; // backups for move undoes
    rytes_t rytes;      //
    move_t GetPMOT() const {
        static const int PMOT_MASK{ 0x1e }; // 0001 1110
        return type & PMOT_MASK;
    }
    bool IsCRON() const {
        return ( type & CRON ) > 0;
    }
    move_t GetCASL() const {
        static const int CASL_MASK{ 0x60 }; // 0110 0000
        return type & CASL_MASK;
    }
    bool IsNPAS() const {
        return ( type & NPAS ) > 0;
    }
    fig_t GetPmotFig( move_t pmot ) const;
    std::string GetStr() const; // e4-e5, f7xd8=N, 0-0, ..
 };
////////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream& ostrm,
                          const Move& mv );
////////////////////////////////////////////////////////
///////////////////////////////////////////////[Painter]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
# include <sstream>
////////////////////////////////////////////////////////
class Painter {
public:
    static std::stringstream draw( const Board& board );
    static std::stringstream paint(const Board& board );
};
////////////////////////////////////////////////////////
///////////////////////////////////////////////////[Com]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
typedef int code_t;
////////////////////////////////////////////////////////
class Com {
private:
    enum { FIN, INSERT, UNDO, SELECT, MOV, SIGSEGV };
    Node * const node;
    std::string nput;
    std::vector<Move> movs;
    //_
    bool Fetch();
    code_t MakMov();
    code_t Undo();
    code_t Select(); // and launch firefox
    code_t Insert
    ( const std::vector<std::string>& args );
    code_t Exec();
public:
    Com( Node * const node ): node( node ){}
    void Launch(); // ha{ha}
};
////////////////////////////////////////////////////////
/////////////////////////////////////////////////[Brush]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
class Brush {
    const std::string prefix;
public:
    Brush( const std::string& f, const std::string& b );
    std::string patron( const std::string& txt ) const;
};
///////////////////////////////////////////////////////_
int main(){
    if( 0 ){
    } else {
        const std::string fen{
            "r3k2r/1P6/8/8/3pP3/8/8/R3K3 b k e3"
        };
        Node * const node{ Node::cons( fen )};
        Com com{ node };
        com.Launch();
    }
}
////////////////////////////////////////////////////////
// +-------------------+ v, > - has
// | Node              | (n) - n items
// v-------v-(2)-------+ Node has a Board and 2 Armies..
// | Board | Army      |
// v-(120)-v-(>=1)-----+
// | SQ    > Unit      |
// |       v-----------+
// | (>=0) > Generator |
// +-------+-----------+
////////////////////////////////////////////////////////
////////////////////////////////////////////////////[SQ]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void SQ::Notify() const {
    // Here the Casl Generator is unsubscribing on King
    // and Rook moves that's why loop over copY.
    auto copY{ subscribers };
    for( Generator * const gen: copY ){
        gen->Update( offset );
    }
}
////////////////////////////////////////////////////////
Unit * SQ::SetUnit( Unit * const nextUnit ){
    Unit * const prevUnit{ this->unit };
    this->unit = nextUnit;
    if( this->unit ){
        this->unit->offset = offset;
    }
    Notify();
    return prevUnit;
}
////////////////////////////////////////////////////////
bool SQ::Check( const color_t color ){
    for( Generator const * const gen: subscribers ){
        if( gen->unit->GetColor() == color ){
            return true;
        }
    }
    return false;
}
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& os, const SQ& sq ){
    static const Brush brush( "128;0;0", "0;0;0" );
    if( sq.subscribers.empty()){
        return os;
    }
    os << brush.patron( Board::GetCoord( sq.offset ))
       << " ";
    for( Generator const * const gen: sq.subscribers ){
        os << gen->unit << " ";
    }
    return os;
}
/////////////////////////////////////////////////[Board]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
offset_t Board::GetOffset( const std::string& sqr ){
    const char file{ sqr[ 0 ]}; // e
    const char rank{ sqr[ 1 ]}; // 4
    const int j{ file - 'a' + FRAME_WIDTH };
    const int i{ PMOT_ROW[ BLACK ] - ( rank - '1' )};
    return GetOffset( i, j );
}
////////////////////////////////////////////////////////
Board::Board() {
    static const int MASK[] = {
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,        
    };
    for( offset_t j{}; j < SIZE; j++ ){
        auto unit{ MASK[ j ] ? Unit::NOP : Unit::NIL };
        sqs.push_back( SQ( j, unit ));
    }
}
////////////////////////////////////////////////////////
Unit * Board::Land( Unit * const unit,
                    const offset_t k ){
    Unit * prev{ sqs[ k ].SetUnit( unit )};
    unit->gen->Subscribe();
    return prev;
}
////////////////////////////////////////////////////////
Unit * Board::LiftOff( const offset_t k ){
    Unit * unit{ sqs[ k ].SetUnit( Unit::NIL )};
    unit->gen->Unsubscribe();
    return unit;
}
////////////////////////////////////////////////////////
Unit * Board::Travel( const offset_t src,
                      const offset_t dst ){
    Unit * const unit{ LiftOff( src )};
    return Land( unit, dst );
}
////////////////////////////////////////////////////////
void Board::Register( Generator * const gen,
                      const offset_t offset ){
    sqs[ offset ].Register( gen );
    gen->Insert( offset );
}
////////////////////////////////////////////////////////
void Board::Unregister( Generator * const gen,
                        const offset_t offset ){
    sqs[ offset ].Unregister( gen );
    gen->Erase( offset );
}
////////////////////////////////////////////////////////
void Board::InLoopUnregister
( Generator * const gen, const offset_t offset ){
    sqs[ offset ].Unregister( gen );
}
////////////////////////////////////////////////////////
color_t Board::GetUnitColor( const offset_t k ) const {
    Unit const * const unit{ GetUnit( k )};
    if( unit->isNIL()){
        return RED;
    }
    if( unit->isNOP()){
        return BLUE;
    }
    return unit->GetColor();
}
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& os, const Board& board ){
    for( int i{ Board::PMOT_ROW[ WHITE ]};
         i <= Board:: PMOT_ROW[ BLACK ]; ++i ){
        for( int j{ Casl::ROOK_COL[ QSIDE ]};
             j <= Casl::ROOK_COL[ KSIDE ]; ++j ){
            const auto k{ Board::GetOffset( i, j )};
            os << board.sqs[ k ];
        }
    }
    return os << std::endl;
}
//////////////////////////////////////////////////[Unit]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
Unit * const Unit::NIL{ nullptr };
Unit * const Unit::NOP{ NIL + 1 };
////////////////////////////////////////////////////////
void Unit::SetFig( const fig_t fig ){
    this->fig = fig;
    delete gen;
    gen = Generator::Factory( this );
}
////////////////////////////////////////////////////////
std::string Unit::GetStr() const {
    static const std::string STR[][ JOKER ]{
        { "k", "q", "r", "b", "n", "p" },
        { "K", "Q", "R", "B", "N", "P" },
    };
    if( isNIL()){
        return " ";
    } else if( isNOP()){
        return "?";
    } else {
        return STR[ color ][ fig ];
    }
}
////////////////////////////////////////////////////////
/////////////////////////////////////////////[Generator]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
# include <set>
////////////////////////////////////////////////////////
Generator::Generator( Unit * const unit ):
    unit( unit ),
    node( unit->node ),
    board( node->GetBoard())
{}
////////////////////////////////////////////////////////
Generator * Generator::Factory( Unit * const unit ){
    switch( unit->GetFig()){
    case KING: {
        return new KingGen( unit );
    }
    case KNIGHT: {
        return new ShortRangeGen<KNIGHT>( unit );
    }
    case QUEEN: {
        return new LongRangeGen<QUEEN>( unit );
    }
    case ROOK: {
        return new LongRangeGen<ROOK>( unit );
    }
    case BISHOP: {
        return new LongRangeGen<BISHOP>( unit );
    }
    case PAWN: {
        if( unit->GetColor()){
            return new PawnGen<WHITE>( unit );
        } else {
            return new PawnGen<BLACK>( unit );
        }
    }
    default: break;
    }
    return {};
}
////////////////////////////////////////////////////////
void Generator::Unsubscribe() {
    // Calling here Unregister will cause problems
    for( const offset_t offset: offsets ){
        board.InLoopUnregister( this, offset );
    }
    offsets.clear();
}
////////////////////////////////////////////////////////
void Generator::GetMoves
( std::vector<Move>& movs ) const {
    const offset_t src{ unit->GetOffset()};
    const color_t srcColor{ unit->GetColor()};
    const offset_t npas{ node->enPassant };
    const rytes_t rytes{ node->rytes };
    for( const offset_t dst: offsets ){
        const color_t dstColor{
            board.GetUnitColor( dst )
        };
        if( dstColor == srcColor ){
            continue;
        }
        move_t type{ dstColor == RED ? MOVE : CRON };
        movs.push_back({ type, src, dst, npas, rytes });
    }
}
////////////////////////////////////////////////////////
std::string Generator::GetStr() const {
    if( offsets.empty()){
        return {};
    }
    // make it sorted
    std::set<offset_t> copY( offsets.begin(),
                             offsets.end());
    std::stringstream ss;
    ss << unit << ":: ";
    for( const offset_t offset: copY ){
        ss << Board::GetCoord( offset ) << " ";
    }
    return ss.str();
}
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& os,
            Generator const * const gen ){
    const auto& s{ gen->GetStr()};
    return s.empty() ? os : os << s << std::endl;
}
////////////////////////////////////////////////////////
/////////////////////////////////////////[ShortRangeGen]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
template <fig_t FIG>
void ShortRangeGen<FIG>::Subscribe(){
    for( int j{}; j < NF_DR; ++j ){
        const offset_t k = unit->GetOffset() + DR[ j ];
        if( board.GetUnitColor( k ) == BLUE ){
            continue;
        }
        board.Register( this, k );
    }
}
////////////////////////////////////////////////////////
//////////////////////////////////////////[LongRangeGen]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
template <fig_t FIG>
void LongRangeGen<FIG>::Scan
( RegisterPtr f, int b, int dr ){
    while( true ){
        b += dr;
        const color_t color{
            board.GetUnitColor( b )
        };
        if( color == BLUE ){
            break;
        }
        // Register v Unregister
        ( board.*f )( this, b );
        if( color != RED ){
            break;
        }
    }
}
template <fig_t FIG>
void LongRangeGen<FIG>::Subscribe(){
    const int a{ unit->GetOffset()};
    RegisterPtr f{ &Board::Register };
    for( int j{ START }; j < NF_DR; j += STEP ){
        int b{ a };
        const int dr{ DR[ j ]}; 
        Scan( f, b, dr );
    }
}
template <fig_t FIG>
void LongRangeGen<FIG>::Update( const offset_t offset ){
    int j{ GetDirection( unit->GetOffset(), offset )};
    const int dr{ DR[ j ]};
    const color_t color{ board.GetUnitColor( offset )};
    int b{ offset };
    RegisterPtr f{
        color == RED ?
        &Board::Register :
        &Board::Unregister
    };
    Scan( f, b, dr );
}
////////////////////////////////////////////////////////
///////////////////////////////////////////////[CaslGen]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void CaslGen::FlipFlop(){
    node->rytes &= ~mask;
}
////////////////////////////////////////////////////////
void CaslGen::Subscribe(){
    for( offset_t j{ kingSrc };; j += kingDr ){
        board.Register( this, j );
        if( j == rookSrc ){
            break;
        }
    }
}
////////////////////////////////////////////////////////
void CaslGen::Update( const offset_t offset ){
    if( offset == kingSrc or offset == rookSrc ){
        Unsubscribe(); // </ Thats
        FlipFlop();
        return;
    }
    // update counters ..
    if( board.GetUnitColor( offset ) == RED ){
        --figuresCounter;
    } else {
        ++figuresCounter;
    }
}
////////////////////////////////////////////////////////
std::string CaslGen::GetStr() const {
    // discards the leading "Ke1:: " stuff
    static const int prefixLen{ 6 };
    const std::string s{ Generator::GetStr()};
    const int n = s.length();
    std::stringstream ss;
    if( n ){
        // discard the trailing space as well
        ss << s.substr( prefixLen, n - prefixLen - 1 ) 
           // and the counter info and a trailing space
           << "' " << figuresCounter << " ";
    }
    return ss.str();
}
////////////////////////////////////////////////////////
bool CaslGen::Go() const {
    const rytes_t rytes{ node->rytes };
    if(!( mask & rytes )){
        return false;
    }
    if( figuresCounter ){
        return false;
    }
    for( offset_t j{ kingSrc };; j += kingDr ){
        if( board.Check( j, !color )){
            return false;
        }
        if( j == kingDst ){
            break;
        }
    }
    return true;
}
////////////////////////////////////////////////////////
///////////////////////////////////////////////[KingGen]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void KingGen::GetMoves
( std::vector<Move>& movs ) const {
    Generator::GetMoves( movs );
    if( casl[ QSIDE ]->Go()){
        movs.push_back({
                LONGCASL,
                0, 0,
                node->enPassant,
                node->rytes });
    }
    if( casl[ KSIDE ]->Go()){
        movs.push_back({
                SHOTCASL,
                0, 0,
                node->enPassant,
                node->rytes });
    }
}
////////////////////////////////////////////////////////
std::string KingGen::GetStr() const {
    std::vector<std::string> caslStr{
        casl[ BLACK ]->GetStr(),
        casl[ WHITE ]->GetStr()
    };
    for( auto& s: caslStr ){
        if( !s.empty()){
            s = " " + s;
        }
    }
    return(
        Generator::GetStr() + " {" +
        caslStr[ BLACK ] + "} [" +
        caslStr[ WHITE ] + "]"
    );
}
///////////////////////////////////////////////[PawnGen]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
template <color_t C>
void PawnGen<C>::Subscribe(){
    const int front{ unit->GetOffset() + FORWARD };
    for( const int dr: { -1, 1 }){
        const int offset{ front + dr };
        const color_t color{
            board.GetUnitColor( offset )
        };
        if( color != BLUE ){
            board.Register( this, offset );
        }
    }
}
///////////////////////////////////////////////////////_
std::vector<Move> Boom( const std::vector<Move>& bufr ){
    std::vector<Move> blowback;
    for( const auto& mv: bufr ){
        for( move_t pmot{ QUEEN_PMOT };
             pmot <= NIGHT_PMOT;
             pmot <<= 1 ){
            Move moov{ mv };
            moov.type |= pmot;
            blowback.push_back( moov );
        }
    }
    return blowback;
}
///////////////////////////////////////////////////////_
template <color_t C> void PawnGen<C>::GetMoves
( std::vector<Move>& movs ) const {
    const offset_t src{ unit->GetOffset()};
    const offset_t npas{ node->enPassant };
    const rytes_t rytes{ node->rytes };
    std::vector<Move> bufr;//bcoz of the promotion blow
    // captures
    for( const auto dst: offsets ){
        const auto dstColor{ board.GetUnitColor( dst )};
        if( dstColor == RED ){
            if( dst == npas ){
                bufr.push_back({ NPAS|CRON, src, dst, npas, rytes });
            }
        } else {
            if( dstColor != unit->GetColor()){
                bufr.push_back({ CRON, src, dst, npas, rytes });
            }
        }
    }
    // movs
    static const int NFS[]{ 1, 2 }; // nf squares
    const int srcRow{ Board::GetRow( src )};
    const int nfs{ NFS[ srcRow == PAWN_ROW ]};
    offset_t dst{ src };
    for( int j{}; j < nfs; ++j ){
        dst += FORWARD;
        const auto dstColor{ board.GetUnitColor( dst )};
        if( dstColor != RED ){
            break;
        }
        bufr.push_back({ MOVE, src, dst, npas, rytes });
    }
    if( !bufr.empty()){
        // promotion
        const int dstRow{ Board::GetRow( dst )};
        if( dstRow == PMOT_ROW ){
            bufr = Boom( bufr ); 
        }
        movs.insert( movs.end(), bufr.begin(), bufr.end());
    }
}
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
    node->Elevate( Casl::GetRytes( vec[ 2 ]));
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
    for( int j{}; rytes; rytes >>= 1, ++j ){
        if( rytes & 1 ){
            const color_t c{ Casl::GetC( j )};
            const flank_t f{ Casl::GetF( j )};
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
////////////////////////////////////////////////////////
Unit * Node::InsertCoin( const char c,
                         const int i,
                         const int j ){
    const color_t color{ Casl::GetColor( c )};
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
void Node::MakeMove( const Move& mov ){
    enPassant = 0;
    move_t isCasl{ mov.GetCASL()};
    if( isCasl ){
        const color_t c{ theSwitch };
        const flank_t f{ isCasl == SHOTCASL };
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
        board.Travel( king_src, king_dst );
        board.Travel( rook_src, rook_dst );
    } else {
        Unit * const unit{
            board.Travel( mov.src, mov.dst )
        };
        // CRON test
        if( unit ){
            army[ unit->GetColor()].unlink( unit );
            unit->gen->Unsubscribe();
        }
    }
    FlipTheSwitch();
}
////////////////////////////////////////////////////////
void Node::UndoMove( const Move& mov ){
    move_t isCasl{ mov.GetCASL()};
    if( isCasl ){
        const color_t c{ !theSwitch };
        const flank_t f{ isCasl == SHOTCASL };
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
        board.Travel( king_dst, king_src );
        board.Travel( rook_dst, rook_src );
    } else {
        board.Travel( mov.dst, mov.src );
        if( mov.type == CRON ){
            Unit * const unit{
                army[ theSwitch ].dance()
            };
            board.Land( unit, mov.dst );
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
              << " " << Casl::GetFEN( node->rytes )
              << std::endl
              << node->army[  node->theSwitch ] 
              << node->army[ !node->theSwitch ];
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//////////////////////////////////////////////////[Army]
Unit * Army::insert( const fig_t type,
                     const color_t color,
                     Node * const node ){
    Unit * u{ king };
    if( type != KING ){
        u = new Unit( type, color, node );
        u->next = king->next;
        king->next = u;
        u->next->prev = u;
        u->prev = king;
    }
    return u;
}
////////////////////////////////////////////////////////
Army::~Army() {
    while( king->next != king ){
        unlink( king->next, true );
    }
    delete king;
    for( Unit * const p: bench ){
        delete p;
    }
}
////////////////////////////////////////////////////////
//   >next>    >next>                            :before
// n        p         q       
//   <prev<    <prev<
////////////////////////////////////////////////////////
//   >next>   ' bench:            >next>          :after
// n        q |        n        p        q
//   <prev<   |          <prev<
////////////////////////////////////////////////////////
void Army::unlink( Unit * const p, const bool del ){
    p->prev->next = p->next;
    p->next->prev = p->prev;
    if( del ){
        delete p;
    } else {
        bench.push_back( p );
    }
}
////////////////////////////////////////////////////////
Unit* Army::dance() {
    Unit* p{ bench.back() };
    bench.pop_back();
    p->prev->next = p->next->prev = p;
    return p;
}
////////////////////////////////////////////////////////
//////////////////////////////////////////////////[Move]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
fig_t Move::GetPmotFig( move_t pmot ) const {
    switch( pmot ){
        case QUEEN_PMOT: return QUEEN;
        case ROOK__PMOT: return ROOK;
        case BSHOP_PMOT: return BISHOP;
        case NIGHT_PMOT: return KNIGHT;
        default:         return KING; 
    }
}
////////////////////////////////////////////////////////
std::string Move::GetStr() const {
    switch( GetCASL()){
        case SHOTCASL: return "0-0";
        case LONGCASL: return "0-0-0"; 
    }
    std::stringstream ss;
    ss << (( IsCRON() or IsNPAS()) ? ":" : "" )
       << Board::GetCoord( dst );
    if( GetPMOT()){
        ss << "="
           << "?QRBN"[ GetPmotFig( GetPMOT())];
    } else if( IsNPAS()){
        ss << "npas";
    }
    return ss.str();
}
////////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream& os,
                          const Move& mv ){
    return os << mv.GetStr();
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
/////////////////////////////////////////////////[Brush]
////////////////////////////////////////////////////////
const std::string CSI{ "\033[" };
////////////////////////////////////////////////////////
// foreground, background
Brush::Brush( const std::string& f,
              const std::string& b ):
    prefix{ CSI + ";38;2;" + f + ";48;2;" + b + ";1m" }
{}
// thats the spit
std::string Brush::patron( const std::string& txt )
    const
{
    static const std::string suffix{ CSI + "0m" };
    return( prefix + txt + suffix );
}
////////////////////////////////////////////////////////
///////////////////////////////////////////////[Painter]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//  1  2  3  4  5  6  7  8     > i ( i - 2 )
// 0123456789abcdefghijklmn    > j ( j - 1 ) * 3 + 1
// -r-'n'-b-'q'-k-'b'-n-'r'0 2 
// 'p'-p-'p'-p-'p'-p-'p'-p-1 3
// ---'''---'''---'''---'''2 4
// '''---'''---'''---'''---3 5
// ---'''---'P'---'''---'''4 6
// '''---'''---'''---'''---5 7
// -P-'P'-P-'''-P-'P'-P-'P'6 8
// 'R'-N-'B'-Q-'K'-B-'N'-R-7 9
std::stringstream Painter::draw( const Board& board ){
    std::vector<std::string> boardCanvas{
        "---'''---'''---'''---'''",
        "'''---'''---'''---'''---",
        "---'''---'''---'''---'''",
        "'''---'''---'''---'''---",
        "---'''---'''---'''---'''",
        "'''---'''---'''---'''---",
        "---'''---'''---'''---'''",
        "'''---'''---'''---'''---",        
    };
    for( int i{ Board::PMOT_ROW[ WHITE ]};
         i <= Board:: PMOT_ROW[ BLACK ]; ++i ){
        const auto u{ i - 2 };
        for( int j{ Casl::ROOK_COL[ QSIDE ]};
             j <= Casl::ROOK_COL[ KSIDE ]; ++j ){
            const auto v{( j - 1 ) * 3 + 1 };
            const auto k{ Board::GetOffset( i, j )};
            const auto unit{ board.sqs[ k ].GetUnit() };
            if( unit->isNIL()){
                continue;
            }
            const char c{ unit->GetStr()[ 0 ]};
            boardCanvas[ u ][ v ] = c;
        }
    }
    std::stringstream ss;
    for( const auto& row: boardCanvas ){
        ss << row << std::endl;
    }
    return ss;
}
////////////////////////////////////////////////////////
std::stringstream Painter::paint( const Board& board ) {
    static const std::string DARK_SQR{ "195;58;20"   };
    static const std::string LYTE_SQR{ "230;150;18"  };
    static const std::string DARK_FIG{ "0;0;0"       };
    static const std::string LYTE_FIG{ "220;250;255" };
    static const Brush DARK[] = {
        Brush( DARK_FIG, DARK_SQR ),
        Brush( LYTE_FIG, DARK_SQR ),
    };
    static const Brush LYTE[] = {
        Brush( DARK_FIG, LYTE_SQR ),
        Brush( LYTE_FIG, LYTE_SQR ),
    };
    std::stringstream ostrm;
    for( int i{ Board::PMOT_ROW[ WHITE ]};
         i <= Board:: PMOT_ROW[ BLACK ];
         ++i ){
        for( int j{ Casl::ROOK_COL[ QSIDE ]};
             j <= Casl::ROOK_COL[ KSIDE ];
             ++j ){
            const auto k{ Board::GetOffset( i, j )};
            const auto unit{ board.sqs[ k ].GetUnit() };
            std::string c{ " " + unit->GetStr() + " " };
            const color_t color{
                unit->isNIL() ?
                WHITE :
                unit->GetColor()
            };
            if(( j^i ) & 1 ){
                ostrm << LYTE[ color ].patron( c );
            } else {
                ostrm << DARK[ color ].patron( c );
            }
        }
        ostrm << std::endl;
    }
    return ostrm;
}
////////////////////////////////////////////////////////
///////////////////////////////////////////////////[Com]
////////////////////////////////////////////////////////
///////////////////////////////////////////////////////=
bool Com::Fetch(){
    static const Brush brush( "0;255;0", "0;127;0" );
    std::cout << brush.patron( "> " );
    return ( bool )std::getline( std::cin, nput );
}
////////////////////////////////////////////////////////
code_t Com::Insert
( const std::vector<std::string>& args ){
    const char c{           args[ 1 ][ 0 ]};
    const int i{ std::stoi( args[ 2 ])    };
    const int j{ std::stoi( args[ 3 ])    };
    node->InsertCoin( c, i, j );
    return INSERT;
}
////////////////////////////////////////////////////////
code_t Com::Exec(){
    if( !Fetch()){
        return FIN;
    }
    auto args{ Node::SplitFEN( nput )};
    const auto& com{ args[ 0 ]};
    if( com == "insert" ){
        return Insert( args );
    } else if( com == "undo" ){
        return Undo();
    } else if( com.size() == 2 ){
        return Select(); // and launch ..
    } else {
        return MakMov();
    }
}
////////////////////////////////////////////////////////
void Com::Launch(){ // firefox
    code_t comsat{ FIN };
    do {
        std::cout <<
            node  <<
            Painter::paint( node->GetBoard()).str();
        comsat = Exec();
        if( comsat == SIGSEGV ){
            std::cout << "Pure virtual function call!\n";
        }
    } while( comsat != FIN );
}
////////////////////////////////////////////////////////
// 0-0
// 0-0-0
// e4e5
// d7e8=Q 
////////////////////////////////////////////////////////
# include <regex>
////////////////////////////////////////////////////////
code_t Com::MakMov(){
    static const std::regex moveRegex{
        "([a-h][1-8])([a-h][1-8])(.*)"
    };
    static const std::regex pmotRegex( "=([QRBN])" );
    move_t type;
    offset_t src{};
    offset_t dst{};
    if( nput == "0-0" ){
        type = SHOTCASL;
    } else if( nput == "0-0-0" ){
        type = LONGCASL;
    } else {
        std::smatch moveMatch;
        std::smatch pmotMatch;
        if( !std::regex_match( nput, moveMatch,
                               moveRegex )){
            return SIGSEGV; // Segmentation Fault
        }
        src = Board::GetOffset( moveMatch[ 1 ].str());
        dst = Board::GetOffset( moveMatch[ 2 ].str());
        const auto& str{ moveMatch[ 3 ].str()};
        if( dst == node->enPassant ){
            type = NPAS;
        } else {
            const color_t color {
                node->GetBoard().GetUnitColor( dst )
            };
            type = ( color != RED );
            if( std::regex_match( str, pmotMatch,
                                  pmotRegex )){
                switch( pmotMatch[ 1 ].str()[ 0 ]){
                case 'Q': type |= QUEEN_PMOT; break;
                case 'R': type |= ROOK__PMOT; break;
                case 'B': type |= BSHOP_PMOT; break;
                case 'N': type |= NIGHT_PMOT; break;
                default: break; // say an error here
                }
            }
        }
    }
    Move mv{ type, src, dst, node->enPassant,
        node->rytes };
    node->MakeMove( mv );
    movs.push_back( mv );
    return MOV;
}
////////////////////////////////////////////////////////
code_t Com::Undo(){
    if( movs.empty()){
        std::cout << "[Empty stack]\n";
    } else {
        node->UndoMove( movs.back());
        movs.pop_back();
    }
    return UNDO;
}
////////////////////////////////////////////////////////
code_t Com::Select(){ // .. your favoirte editor
    static const std::regex regex( "([a-h][1-8])" );
    std::smatch match;
    if( !std::regex_match( nput, match, regex )){
        std::cout << "Input: " << nput
                  << ", didn't match\n";
    }
    int k{ Board::GetOffset( match[ 1 ].str()) };
    Unit const * const unit{ 
        node->GetBoard().GetUnit( k )
    };
    if( !unit->isNIL()){
        std::vector<Move> mv;
        unit->gen->GetMoves( mv );
        std::cout << unit << mv << " ";
        mv.clear();
    }
    for( const color_t color: { BLACK, WHITE }){
        const bool check{
            node->GetBoard().Check( k, color )
        };
        std::cout << "{" << "-+"[ check ] << "} "; 
    }
    std::cout << std::endl;
    return SELECT;
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
// DOTO:
