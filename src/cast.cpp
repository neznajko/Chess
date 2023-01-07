                            ////////
///////////////////////////////////////////////////////////////
# include "node.hpp" 
///////////////////////////////////////////////////////////////
const int Cast::KING_SQUARE[ RED ] = {
    Board::getk( "e8" ), Board::getk( "e1" )
};
const int Cast::ROOK_SQUARE[ RED ][ CENTER ] = {
    { Board::getk( "a8" ), Board::getk( "h8" )},
    { Board::getk( "a1" ), Board::getk( "h1" )},
};
// define landing squares for king and the two rooks
const int Cast::KING_LANDING_SQUARE[ RED ][ CENTER ] = {
    { Board::getk( "c8" ), Board::getk( "g8" )},
    { Board::getk( "c1" ), Board::getk( "g1" )},
};
const int Cast::ROOK_LANDING_SQUARE[ RED ][ CENTER ] = {
    { Board::getk( "d8" ), Board::getk( "f8" )},
    { Board::getk( "d1" ), Board::getk( "f1" )},
};
///////////////////////////////////////////////////////////////_
inline Unit* Cast::getKing( color_t color ) const {
    return node->army[ color ].king;
}
///////////////////////////////////////////////////////////////=
inline Unit* Cast::getRook( color_t color, flank_t flank ) const
{
    int k{ ROOK_SQUARE[ color ][ flank ]};
    return node->brd[ color ].getunit( k );
}
////////////////////////////////////////////////////////////////
void Cast::setmoos( const std::string& fen, color_t color ){
    static const std::string SMBL[]{ "qk", "QK" };
    for( int j = QUEENSIDE; j < CENTER; j++ ){
        if( fen.find( SMBL[ color ][ j ] ) != std::string::npos ){
            getKing( color    )->noofmoos = 0;
            getRook( color,( flank_t )j )->noofmoos = 0;
        }
    }
}
////////////////////////////////////////////////////////////////
bool Cast::getrytes( color_t color, flank_t flank ) const {
    // if the King has moo, return immediately
    if( getKing( color )->noofmoos ) return false;
    // Othervise check vhether rook has mood.
    return( getRook( color, flank )->noofmoos == 0 );
}
void Cast::make_cast( flank_t flank ){ /////////////////////////
    color_t acolor{ node->GetActvColor() };
    node->travel( KING_SQUARE[ acolor ],
                  KING_LANDING_SQUARE[ acolor ][ flank ]);
    node->travel( ROOK_SQUARE[ acolor ][ flank ],
                  ROOK_LANDING_SQUARE[ acolor ][ flank ]);
}
void Cast::unmake_cast( flank_t flank ){ ///////////////////////
    // White has moved 0-0-0 now Black to move, your task unmove
    // castles on the queen side, the prfix p here means passive.
    color_t pcolor{ node->GetPasvColor() };
    node->untravel( KING_LANDING_SQUARE[ pcolor ][ flank ],
                    KING_SQUARE[ pcolor ]);
    node->untravel( ROOK_LANDING_SQUARE[ pcolor ][ flank ],
                    ROOK_SQUARE[ pcolor ][ flank ]);
}
////////////////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream& ostrm, const Cast& cast ){
    return ostrm << "Castl: BLACK("
                 << cast.getrytes( BLACK, QUEENSIDE ) << ","
                 << cast.getrytes( BLACK,  KINGSIDE ) << "), "
                 << "WHITE("
                 << cast.getrytes( WHITE, QUEENSIDE ) << ","
                 << cast.getrytes( WHITE,  KINGSIDE ) << ")";
}
///////////////////////////////////////////////////////////////
////////                                               ////////
