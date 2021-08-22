////////////////////////////////////////////////////////
#if !defined cast_hpp
#define cast_hpp

#include <bitset>

#include "move.hpp"
////////////////////////////////////////////////////////
class Castles { // on the Queen Side
private: // KQkq
    int8_t rytes_; // BLACK / WHITE   QUEENSIDE / KINGSIDE
    static int bitmask( const bool color, const bool flank )
    // ( WHITE, KINGSIDE ) => 1000H
    {
        return 1 << (( color << 1 ) | flank );
    }
public:
    void setRyte( const bool color, const bool flank )
    {
        rytes_ |= bitmask( color, flank );
    }
    bool getRyte( const bool color, const bool flank ) const
    {
        return 0 < ( rytes_ & bitmask( color, flank ));
    }
    std::string bits() const
    {
        return std::bitset<4>( rytes_ ).to_string();
    }
    void setFenRytes( const std::string& fenRytes );
    moves_t getCastles( const class Position& pos ) const;
};
////////////////////////////////////////////////////////
// log:
#endif// cast_hpp
////////////////////////////////////////////////////////
