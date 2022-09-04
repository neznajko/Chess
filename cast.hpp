////////////////////////////////////////////////////////
# if !defined cast_hpp
# define cast_hpp

# include <bitset>
////////////////////////////////////////////////////////
class Coor;
class King;
////////////////////////////////////////////////////////
class Castles { // on the Queen Side
private: 
    u_int8_t rytes_; // KQkq
    static int bitmask( const bool color,
                        const bool flank ){
        // bitmask( 1, 0 ) => 0100
        const int shift = ( color << 1 )|flank;
        return 1 << shift;
    }
public:
    enum { QWENSIDE, KINGSIDE };
    Castles(): rytes_( 0 ){}
    void setRyte( const bool color, const bool flank ){
        rytes_ |= bitmask( color, flank );
    }
    bool getRyte( const bool color, const bool flank ) const {
        return 0 < ( rytes_ & bitmask( color, flank ));
    }
    std::string bits() const {
        return std::bitset<4>( rytes_ ).to_string();
    }
    void setFenRytes( const std::string& fenRytes );
    //
    Coor check_KingSide( King* king );
};
////////////////////////////////////////////////////////
# endif
////////////////////////////////////////////////////////
