                          ////
////////////////////////////////////////////////////////
# include <sstream>
////////////////////////////////////////////////////////
# include "node.hpp"
////////////////////////////////////////////////////////
const Move Move::Fin = Move( NPAS, -1, -1 );
////////////////////////////////////////////////////////
fig_t Move::get_pmot_fig( type_t pmot ) const
{
    switch( pmot ){
    case QUEEN_PMOT:  return Figure::QUEEN;
    case ROOK_PMOT:   return Figure::ROOK;
    case BISHOP_PMOT: return Figure::BISHOP;
    case KNIGHT_PMOT: return Figure::KNIGHT;
    default:          return Figure::NIL; }
}
////////////////////////////////////////////////////////
std::string Move::str() const
{
    switch( get_cast() ){
    case SHORT_CAST: return "0-0";
    case LONG_CAST:  return "0-0-0"; }
    std::stringstream ss;
    ss << Board::coor( sorc )
       << (( is_cron() or is_npas()) ? ":" : "-" )
       << Board::coor( dest );
    if( get_pmot() ){
        ss << "="
           << "?QRBN"[ get_pmot_fig( get_pmot()) ];
    } else if( is_npas()) {
        ss << "npas";
    }
    return ss.str();
}
////////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream& os, const Move& mv ){
    return os << mv.str();
}
////////////////////////////////////////////////////////
// log:
////////////////////////////////////////////////////////
////                                                ////
