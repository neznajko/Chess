////////////////////////////////////////////////////////////////
# include <regex>
////////////////////////////////////////////////////////////////
# include "com.hpp"
////////////////////////////////////////////////////////////////
// 0-0
// 0-0-0
// e4e5
// d7e8=Q
// e5d6npas
////////////////////////////////////////////////////////////////
Move Com::GetMov() {
    static const std::regex move_regex( "([a-h][1-8])([a-h][1-8])(.*)" );
    static const std::regex pmot_regex( "=([QRBN])" );
    if( nput == "0-0" ){
        return Move( SHORT_CAST ); 
    } else if( nput == "0-0-0" ){
        return Move( LONG_CAST );
    }
    std::smatch move_match;
    std::smatch pmot_match;
    if( !std::regex_match( nput, move_match, move_regex )){
        return Move( MOVE, 40, 41 ); // report an error
    }
    int sorc{ Board::getk( move_match[ 1 ].str()) };
    int dest{ Board::getk( move_match[ 2 ].str()) };
    Move::type_t type;
    const auto& str{ move_match[ 3 ].str() };
    const auto acolor{ node->GetActvColor() };
    if( str == "npas" ){
        dest = Board::npas_convyort( dest, acolor );
        type = NPAS;
    } else {
        ////////////////////////////////////////////////////////
        // if the destination square, from the pasv board is not
        // empty that is CRON othervise it's just a MOVE
        Unit* u{ node->pasv().getunit( dest )};
        type = u->isnil() ? MOVE : CRON;
        if( std::regex_match( str, pmot_match, pmot_regex )){
            switch( pmot_match[ 1 ].str()[ 0 ]){
                case 'Q': type |= QUEEN_PMOT; break;
                case 'R': type |= ROOK_PMOT; break;
                case 'B': type |= BISHOP_PMOT; break;
                case 'N': type |= KNIGHT_PMOT; break;
                default: break; // report an error here
            }
        }
    }
    return Move( type, sorc, dest );
}
///////////////////////////////////////////////////////////////=
bool Com::exec() {
    if(! Fetch()) return false;
    if( nput == "undo" ){
        undo();
    } else {
        node->make_move( GetMov());
    }
    return true;
}
///////////////////////////////////////////////////////////////_
void Com::undo() {
    node->unmake_move();
}
////////////////////////////////////////////////////////////////
//
//
