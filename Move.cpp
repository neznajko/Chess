////////////////////////////////////////////////////////
# include "Move.hpp"
# include "Board.hpp"
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
std::string Move::GetStr( const bool long_fmt ) const {
    switch( GetCASL()){
        case SHOTCASL: return "0-0";
        case LONGCASL: return "0-0-0"; 
    }
    std::stringstream ss;
    if( long_fmt ){
        ss << Board::GetCoord( src );
    }
    ss << ( IsCRON() ? ":" : "" )
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
// consider here long and short versions
