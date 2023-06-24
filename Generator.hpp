////////////////////////////////////////////////////////
# ifndef GENERATOR_HPP
# define GENERATOR_HPP
////////////////////////////////////////////////////////
# include "Unit.hpp"
/////////////////////////////////////////////[Generator]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
struct Move;
///////////////////////////////////////////////////////=
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
    virtual ~Generator(){
#   ifdef DEBUG
        std::cout << "::<< " << this->unit << endl;
#   endif        
    }
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
    const offset_t kingSrc;
    const offset_t rookSrc;
    const int mask;
    const std::vector<offset_t> checkSqs;
    const offset_t rookCheckSq;
public:
    CaslGen( Unit * const king, const flank_t flank ):
        Generator( king ),
        color( king->GetColor()),
        kingSrc( Casl::KING_FROM[ color ]),
        rookSrc( Casl::ROOK_FROM[ color ][ flank ]),
        mask( Casl::Mask( color, flank )),
        checkSqs( Casl::CHECK_SQS[ color ][ flank ]),
        rookCheckSq( Casl::ROOK_CHECK_SQ[ color ][ flank ])
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
              new CaslGen( unit, KSIDE )}{}
    ~KingGen(){
#    ifdef DEBUG        
        std::cout << "KingGen out\n";
#    endif
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
    static const int FORWARD{ Board::FORWARD[ C ]};
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
# endif
///////////////////////////////////////////////////////_
