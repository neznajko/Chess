////////////////////////////////////////////////////////
# include "cast.hpp"
# include "node.hpp"
////////////////////////////////////////////////////////
void Castles::setFenRytes( const std::string& fenRytes ){
    if( fenRytes[ 0 ] == '-' ){
        return;
    }
    for( const char c: fenRytes ){        
        switch( c )
        {
            case 'K': setRyte( WHITE, KINGSIDE ); break;
            case 'Q': setRyte( WHITE, QWENSIDE ); break;
            case 'k': setRyte( BLACK, KINGSIDE ); break;
            case 'q': setRyte( BLACK, QWENSIDE ); break;
            default:  std::cerr << "On Y Va!!,\n";
        }
    }
}
////////////////////////////////////////////////////////
Coor Castles::check_KingSide( King* king ){
    // Black: e8 f8 g8 h8
    // White: e1 f1 g1 h1
    const static Coor f[] = { Coor( "f8" ), Coor( "f1") };
    const static Coor g[] = { Coor( "g8" ), Coor( "g1") };
    Board* brd = king->brd_;
    // [0] get King's color
    bool color = king->color();
    // [1] fst check position's rytes
    if( !getRyte( color, KINGSIDE )) return {};
    // [2] aura
    // +----+----+----+
    // | NW | N  | NE |
    // +----+----+----+
    // |  W |    |  E | << the f-square
    // +----+----+----+
    // | SW | S  | SE |
    // +----+----+----+
    if( !king->aura[ Coor::E ]) return {};
    // [3] f-square
    Unit* u = brd->getUnit( f[ color ]);
    if( u != Unit::ZILCH ) return {};
    // [4] g-square
    return {};
}
////////////////////////////////////////////////////////
