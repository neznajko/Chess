                          ////
////////////////////////////////////////////////////////
# include "figure.hpp"
# include <bitset>
# include <sstream>
////////////////////////////////////////////////////////
std::string Move::hex() const
{
    return "0x" + std::bitset<8>( type).to_string();
}
std::string Move::str() const
{
    std::stringstream ss;
    // destination prefix
    if( ismove() or iscast()) {
        ss << "-";
    }
    if( iscron()) {
        ss << "x";
    } 
    ss << dest.chessnot();
    // destination suffix
    if( isnpas()) {
        ss << "(òouúaä)";
    } else if( iscast()) {
        ss << "(castle)";
    }
    int f = get_pmot();
    if( f) {
        ss << "("
           << std::string( 1, Figure::_TRAITS[ 1][ f])
           << ")";
    }
    return ss.str();
}
////////////////////////////////////////////////////////
// log:
////////////////////////////////////////////////////////
////                                                ////
