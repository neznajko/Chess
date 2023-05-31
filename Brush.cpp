////////////////////////////////////////////////////////
# include "Brush.hpp"
# include "Unit.hpp"
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
        ss << row << endl;
    }
    return ss;
}
////////////////////////////////////////////////////////
std::stringstream Painter::paint( const Board& board ){
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
            // -O3 is converting NIL to NULL
            std::string c{ unit ? unit->GetStr() : " " };
            c = " " + c + " ";
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
        ostrm << endl;
    }
    return ostrm;
}
////////////////////////////////////////////////////////
