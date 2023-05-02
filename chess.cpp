////////////////////////////////////////////////////////
# include <vector>
# include <iostream>
////////////////////////////////////////////////////////
template <typename T>
std::ostream& operator<<( std::ostream& ostrm,
                          const std::vector<T>& vec ){
    ostrm << "[";
    for( int j = 0, n = vec.size(); j < n; ++j ){
        ostrm << " " << vec[ j ] << " ,"[ j < n - 1 ];
    }
    return ostrm << "]";
}
////////////////////////////////////////////////////////
# include <chrono>
////////////////////////////////////////////////////////
void benchmark( void (*f)(), int NfLoops ){
    using namespace std::chrono;
    const auto start{ steady_clock::now() };
    while( NfLoops-- > 0 ){
        f();
    }
    const auto stop{ steady_clock::now() };
    duration<double> total = stop - start;
    std::cout << total.count() << " sec\n";
}
////////////////////////////////////////////////////////
# include <unordered_set>
////////////////////////////////////////////////////////
////////////////////////////////////////////////////[SQ]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
class Unit;
class Generator;
////////////////////////////////////////////////////////
typedef char offset_t;
////////////////////////////////////////////////////////
class SQ {
    const offset_t offset;
    Unit * unit;
    std::unordered_set<Generator *> subscribers;
    void Notify() const;
public:
    SQ( const offset_t offset,
        Unit * unit ):
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
enum { BLACK, WHITE, RED, BLUE };
enum { QSIDE, KSIDE, CENTER };
////////////////////////////////////////////////////////
typedef int color_t;
typedef bool flank_t;
////////////////////////////////////////////////////////
namespace Casl {
    static constexpr int __KINGS__COL{ 5 };
    static constexpr int ROOKS_COL[]{ 1, 8 };
    static constexpr int __KINGS__DR[]{ -1, 1 };
    static constexpr int __KINGS__SQS{ 2 };
    static constexpr int __ROOKS__SQS[]{ 3, 2 };
};
////////////////////////////////////////////////////////
class Board {
    std::vector<SQ> sqs;
public:
    static constexpr int WIDTH{ 10 };
    static constexpr int __HEIGHT{ 12 };
    static constexpr int SIZE{ WIDTH * __HEIGHT };
    static constexpr int FRAME_WIDTH{ 1 };
    static constexpr int __FRAME__HEIGHT{ 2 };
    static constexpr int PMOT_ROW[]{ 9, 2 };
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
    void Register( Generator * const gen,
                   const offset_t offset );
    void Unregister( Generator * const gen,
                     const offset_t offset );
    void InLoopUnregister( Generator * const gen,
                           const offset_t offset );
    color_t GetUnitColor( const offset_t k ) const;
    friend std::ostream&
    operator<<( std::ostream& os, const Board& board );
    friend class Painter;
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
    fig_t    fig; // can change via promotn
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
    Generator * gen = nullptr; // changing under promotn
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
    static constexpr int NF_SHORTRANGE_DR{ 8 };
    static constexpr offset_t
    SHORTRANGE_DR[][ NF_SHORTRANGE_DR ]{ // King
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
    virtual void GetMoves( std::vector<Move>& movs )
        const;
    virtual std::string Str() const;
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
    static constexpr int
    NF_DR{ NF_SHORTRANGE_DR };
    static constexpr offset_t const * const DR{
        Generator::SHORTRANGE_DR[ FIG == KNIGHT ]
    };
public:
    ShortRangeGen( Unit * const unit ):
        Generator( unit ) {}
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
    static constexpr int NF_DR{ NF_SHORTRANGE_DR };
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
template <flank_t F>
class CaslGen: public Generator {
    static const int
    ROOKS_COL{ Casl::ROOKS_COL[ F ]};
    static const int
    __KINGS__DR{   Casl::__KINGS__DR[ F ]};
    static const int
    __ROOKS__DR{ -           __KINGS__DR };
    static const int
    __ROOKS__SQS{ Casl::__ROOKS__SQS[ F ]};
public:
    CaslGen( Unit * const king ):
        Generator( king ) {}
    int GetCaslRow() const {
        return Board::PMOT_ROW[ !unit->GetColor()];
    }
    void Subscribe();
    void Update( const offset_t offset );
    void GetMoves( std::vector<Move>& movs )
        const override;
};
////////////////////////////////////////////////////////
///////////////////////////////////////////////[KingGen]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
class KingGen: public ShortRangeGen<KING> {
    Generator * const casl[ CENTER ];
public:
    KingGen( Unit * const unit ):
        ShortRangeGen<KING>( unit ),
        casl{ new CaslGen<QSIDE>( unit ),
              new CaslGen<KSIDE>( unit )}
        {}
    ~KingGen() {
        for( const auto flank: { QSIDE, KSIDE }){
            delete casl[ flank ];
        }
    }
    void Subscribe();
    void Update( const offset_t offset );
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
# include <cctype>
# include <map>
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//////////////////////////////////////////////////[Node]
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
    static color_t GetColor( const char c ){
        return std::isupper( c ) != 0;
    }
public:
    offset_t enPassant{};
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
    move_t   type; 
    offset_t src;  
    offset_t dst;  
    offset_t npas; // a backup for move undoing
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
    std::string Str() const; // e4-e5, f7xd8=N, 0-0, ..
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
        const std::string fen{ "k7/8/8/8/8/8/8/8 b" };
        Node * const node{ new Node()};
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
    for( Generator * const gen: subscribers ){
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
std::ostream&
operator<<( std::ostream& os, const SQ& sq ){
    if( sq.subscribers.empty()){
        return os;
    }
    os << Board::GetCoord( sq.offset ) << ":= ";
    for( Generator const * const gen: sq.subscribers ){
        os << gen->unit << " ";
    }
    return os << std::endl;
}
/////////////////////////////////////////////////[Board]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
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
void Board::InLoopUnregister
( Generator * const gen, const offset_t offset ){
    sqs[ offset ].Unregister( gen );
}
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
color_t Board::GetUnitColor( const offset_t k ) const {
    Unit const * const unit{ sqs[ k ].GetUnit() };
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
        for( int j{ Casl::ROOKS_COL[ QSIDE ]};
             j <= Casl::ROOKS_COL[ KSIDE ]; ++j ){
            const auto k{ Board::GetOffset( i, j )};
            os << board.sqs[ k ];
        }
    }
    return os;
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
    int i{ Board::__FRAME__HEIGHT };
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
    /*
    for( auto color: { BLACK, WHITE }){
        node->setMoos( vec[ 2 ], color );
    }
    if( vec[ 3 ] != "-" ){
        node->setEnPassant( Board::getK( vec[ 3 ]));
    }*/
    return node;
}
////////////////////////////////////////////////////////
Unit * Node::InsertCoin( const char c,
                         const int i,
                         const int j ){
    const color_t color{ GetColor( c )};
    const fig_t fig{ GetFig( c )};
    const offset_t k{ Board::GetOffset( i, j )};
    Unit * u{ army[ color ].insert( fig, color, this )};
    board.Land( u, k );
    return u;
}
////////////////////////////////////////////////////////
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
    Unit * unit{ board.LiftOff( mov.src )};
    unit = board.Land( unit, mov.dst );
    // CRON test
    if( unit ){
        army[ unit->GetColor()].unlink( unit );
        unit->gen->Unsubscribe();
    }
    FlipTheSwitch();
}
////////////////////////////////////////////////////////
void Node::UndoMove( const Move& mov ){
    Unit * unit{ board.LiftOff( mov.dst )};
    board.Land( unit, mov.src );
    if( mov.type == CRON ){
        unit = army[ !unit->GetColor()].dance();
        board.Land( unit, mov.dst );
    }
    FlipTheSwitch();
}
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& os, Node const * const node ){
    return os << node->board
              << node->army[  node->theSwitch ] 
              << node->army[ !node->theSwitch ];
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
void Generator::GetMoves( std::vector<Move>& movs )
    const {
    for( auto offset: offsets ){
        const color_t dstColor{
            board.GetUnitColor( offset )
        };
        const color_t srcColor{
            unit->GetColor()
        };
        if( dstColor == srcColor ){
            continue;
        }
        move_t type{
            ( dstColor == RED ) ? MOVE : CRON
        };
        movs.push_back({
                type,
                unit->GetOffset(),
                offset,
                node->enPassant
            });            
    }
}
////////////////////////////////////////////////////////
std::string Generator::Str() const {
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
    const auto& s{ gen->Str()};
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
template <flank_t F>
void CaslGen<F>::Subscribe() {
}
////////////////////////////////////////////////////////
template <flank_t F>
void CaslGen<F>::Update( const offset_t offset ){
}
////////////////////////////////////////////////////////
template <flank_t F>
void CaslGen<F>::GetMoves( std::vector<Move>& movs )
    const {
}
////////////////////////////////////////////////////////
///////////////////////////////////////////////[KingGen]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void KingGen::Subscribe() {
    ShortRangeGen<KING>::Subscribe();
}
////////////////////////////////////////////////////////
void KingGen::Update( const offset_t offset ){
}
////////////////////////////////////////////////////////
void KingGen::GetMoves( std::vector<Move>& movs )
    const {
    Generator::GetMoves( movs );
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
std::string Move::Str() const {
    switch( GetCASL()){
        case SHOTCASL: return "0-0";
        case LONGCASL: return "0-0-0"; 
    }
    std::stringstream ss;
    ss << Board::GetCoord( src )
       << (( IsCRON() or IsNPAS()) ? ":" : "-" )
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
    return os << mv.Str();
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
        for( int j{ Casl::ROOKS_COL[ QSIDE ]};
             j <= Casl::ROOKS_COL[ KSIDE ]; ++j ){
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
        for( int j{ Casl::ROOKS_COL[ QSIDE ]};
             j <= Casl::ROOKS_COL[ KSIDE ];
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
    // static const std::regex
    //     pmotRegex( "=([QRBN])" );
    // if( nput == "0-0" ){
    //     return Move( SHOTCASL, 0,
    //                  node->getActvArmy().king ); 
    // } else if( nput == "0-0-0" ){
    //     return Move( LONGCASL, 0,
    //                  node->getActvArmy().king ); 
    //  }
    std::smatch moveMatch;
    // std::smatch pmotMatch;
    if( !std::regex_match( nput, moveMatch,
                           moveRegex )){
        return SIGSEGV; // Segmentation Fault
    }
    const offset_t src{
        Board::GetOffset( moveMatch[ 1 ].str())
    };
    const offset_t dst{
        Board::GetOffset( moveMatch[ 2 ].str())
    };
    // const auto& str{ moveMatch[ 3 ].str() };
    move_t type;
    if( 0 /*node->isEnPassant( dest )*/){
    //     type = NPAS;
    } else {
        const color_t color {
            node->GetBoard().GetUnitColor( dst )
        };
        type = color == RED ? MOVE : CRON;
    //     if( std::regex_match( str, pmotMatch,
    //                           pmotRegex )){
    //         switch( pmotMatch[ 1 ].str()[ 0 ]){
    //             case 'Q': type |= QUEEN_PMOT; break;
    //             case 'R': type |= ROOK__PMOT; break;
    //             case 'B': type |= BSHOP_PMOT; break;
    //             case 'N': type |= NIGHT_PMOT; break;
    //             default: break; // brag an error here
    //         }
    //     }
    }
    Move mv{ type, src, dst, node->enPassant };
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
    /*
    static const std::regex regex( "([a-h][1-8])" );
    std::smatch match;
    if( !std::regex_match( nput, match, regex )){
        std::cout << "Input: " << nput
                  << ", didn't match\n";
    }
    int k{ Board::getK( match[ 1 ].str()) };
    Unit* u{ node->getActvUnit( k )};
    if( u->isNIL()) {
        std::cout << "Not an active figure\n";
    } else {
        std::cout << *u << " " << u->getStk() << endl;
        unit = u;
    }
    */
    return SELECT;
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
// DOTO: - Remoo __ form static const stuff
//
