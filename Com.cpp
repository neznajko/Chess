////////////////////////////////////////////////////////
# include "Com.hpp"
# include "Brush.hpp"
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
    } else if( com == "perft" ){
        return Perft( std::stoi( args[ 1 ]));
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
        if( comsat == SEGMENTATIONFAULT ){
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
            return SEGMENTATIONFAULT;
        }
        src = Board::GetOffset( moveMatch[ 1 ].str());
        dst = Board::GetOffset( moveMatch[ 2 ].str());
        const auto& str{ moveMatch[ 3 ].str()};
        if( dst == node->enPassant ){
            type = NPAS|CRON;
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
    // patch #82fa, update type if pawn double mov
    Unit const * const unit{
        node->GetBoard().GetUnit( src )
    };
    if( type == MOVE and
        unit->GetFig() == PAWN ){
        if( std::abs( dst - src ) > Board::WIDTH ){
            type |= NPAS;
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
    std::cout << endl;
    return SELECT;
}
////////////////////////////////////////////////////////
code_t Com::Perft( const int depth ){
    std::cout << "perft " << depth << ": "
              << node->Perft( depth ) << endl;
    return PERFTECT;
}
////////////////////////////////////////////////////////
// - consider here a position command
// - better throw a volley at fin rather than returning
//   a code
