                            ////////
///////////////////////////////////////////////////////////////
# ifndef CAST_HPP
# define CAST_HPP
///////////////////////////////////////////////////////////////
# include <string>
# include <iostream>
///////////////////////////////////////////////////////////////
typedef enum { QUEENSIDE, KINGSIDE, CENTER } flank_t;
///////////////////////////////////////////////////////////////
typedef enum { BLACK, WHITE, RED } color_t;
///////////////////////////////////////////////////////////////
class Cast {
private:
    static const int KING_SQUARE[ RED ];
    static const int ROOK_SQUARE[ RED ][ CENTER ];
    static const int KING_LANDING_SQUARE[ RED ][ CENTER ];
    static const int ROOK_LANDING_SQUARE[ RED ][ CENTER ];
    struct Node* node;
    Unit* getKing( color_t color ) const;
    Unit* getRook( color_t color, flank_t flank ) const;
public:
    Cast( Node* node_ ): node( node_ ) {}
    void setmoos( const std::string& fen, color_t color );
    bool getrytes( color_t color, flank_t flank ) const;
    void make_cast( flank_t flank );
    void unmake_cast( flank_t flank );
};
////////////////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream& ostrm, const Cast& cast );
////////////////////////////////////////////////////////////////
# endif
////////////////////////////////////////////////////////////////
////////                                                ////////
