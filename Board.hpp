////////////////////////////////////////////////////////
# ifndef BOARD_HPP
# define BOARD_HPP
////////////////////////////////////////////////////////
# include "SQ.hpp"
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
    ///////////////////////////////////////////////////_
    // B = 0, Q = 0 0 - 00 BQ C = { B, W }
    // W = 1, K = 1 1 - 01 BK F = { Q, K }
    //              2 - 10 WQ J = { 0, 1, 2, 3 }
    //              3 - 11 WK J = C << 1 + F
    // M = { 1000, 0100, 0010, 0001 }, M = 1 << J
    ///////////////////////////////////////////////////=
    // FEN + CF + J + MASK     8 4 2 1         R Y T E S
    //  k  - BK - 1 -  2       K Q k q
    //  q  - BQ - 0 -  1
    //  K  - WK - 3 -  8
    //  Q  - WQ - 2 -  4
    ///////////////////////////////////////////////////-
    inline color_t Color( const char c ){
        return std::isupper( c ) != 0;
    }
    inline flank_t Flank( const char c ){
        return std::tolower( c ) == 'k'; 
    }
    inline color_t C( const int j ){
        return j >> 1;
    }
    inline flank_t F( const int j ){
        return j & 1;
    }
    inline int J( const color_t c, const flank_t f ){
        return ( c << 1 ) + f;
    }
    inline int Mask( color_t c, flank_t f ){
        return 1 << J( c, f );
    }
    inline int Mask( const char c ){
        return Mask( Color( c ), Flank( c ));
    }
    inline rytes_t Rytes( const std::string& fen ){
        rytes_t rytes{};
        if( fen != "-" ){
            for( const char c: fen ){
                rytes |= Mask( c );
            }
        }
        return rytes;
    }
    inline std::string FEN( rytes_t rytes ){
        static const char FIG[]{ "qkQK" };
        std::string fen;
        for( int j{}; rytes; rytes >>= 1, ++j ){
            if( rytes & 1 ){
                fen.push_back( FIG[ j ]);
            }
        }
        return fen.empty() ? "-" : fen;
    }
}
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
    static constexpr int FORWARD[]{ WIDTH, -WIDTH };
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
    Unit * Promote( const fig_t fig,
                    const offset_t src,
                    const offset_t dst );
    void Register( Generator * const gen,
                   const offset_t offset );
    void Unregister( Generator * const gen,
                     const offset_t offset );
    void InLoopUnregister( Generator * const gen,
                           const offset_t offset );
    Unit const * GetUnit( const offset_t k ) const {
        return sqs[ k ].GetUnit();
    }
    color_t GetUnitColor( const offset_t k ) const;
    bool Check( const offset_t k,
                const color_t color ) const {
        return sqs[ k ].Check( color );
    }
    friend class Painter;
    friend std::ostream&
    operator<<( std::ostream& os, const Board& board );
};
////////////////////////////////////////////////////////
# endif
////////////////////////////////////////////////////////
// log: - Consider removing InLoopUnregister, and copy
// offsets before the loop.
