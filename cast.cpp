////////////////////////////////////////////////////////
#include <iostream>

#include "position.hpp"

enum { QEENSIDE, KINGSIDE };
////////////////////////////////////////////////////////
void Castles::setFenRytes( const std::string& fenRytes )
{
    if( fenRytes[0] == '-' ) return;
    //    
    for( const char c: fenRytes ){
        
        switch( c )
        {
            case 'K': setRyte( WHITE, KINGSIDE ); break;
            case 'Q': setRyte( WHITE, QEENSIDE ); break;
            case 'k': setRyte( BLACK, KINGSIDE ); break;
            case 'q': setRyte( BLACK, QEENSIDE ); break;
            default:  std::cerr << "On Y Va!!,\n";
        }
    }
}
moves_t Castles::getCastles( const Position& pos ) const
{
    // Squares through which the King should pass and land.
    static const std::vector<Coor> SQRS[][ RED ] = {
        // CaStLe's SQuaRes
        {{ Coor( "d8" ), Coor( "c8" )},  //
         { Coor( "f8" ), Coor( "g8" )}}, //
        {{ Coor( "d1" ), Coor( "c1" )},
         { Coor( "f1" ), Coor( "g1" )}}
    };
    // Staäting squares.
    static const Coor SORS[] = {
        Coor( "e8" ), Coor( "e1" )
    };    
    static const Coor NSQR[] = { // kNight SQuaRes
        Coor( "b8" ), Coor( "b1" )
    };
    static const int KOFF[] = { -1, 0}; // King offsets
    //
    const bool color { pos.get_active_color() };
    // First of all Kings should be on their staäting
    // sqares ..
    if( pos.get_board().get_offset( SORS[ color]) !=
        KOFF[ color]) {
        return {/* this */};
    }
    // .. and not in check.
    if( pos.undafire( SORS[ color])) return {};
    moves_t moves;
    for( const bool flank: { QEENSIDE, KINGSIDE })
    {
        if( !getRyte( color, flank )) continue;        
        // ck Knight squares for 0-0-0
        if( flank == QEENSIDE) {
            if( pos.get_board().get_offset( NSQR[ color])
                != EMPTY) continue;
        }
        // ck if King's route is safe and clear
        bool pass{ true };
        for( const Coor& r: SQRS[ color ][ flank ])
        {
            if( pos.get_board().get_offset(r) != EMPTY ){
                pass = false;
                break;
            }
            if( pos.undafire( r ))
            {
                pass = false;
                break;
            }
        }
        if( pass ){
            const Coor& sors{ SORS[ color ]};
            const Coor& dest{ SQRS[ color ][ flank ][1] };
            moves.emplace_front( sors, dest, CAST );
        }
    }
    return moves;
}
////////////////////////////////////////////////////////
// log: - ck for friendly figures                    [v]
//      - the King should be at starting position    [v]
// - Long castle with Nb8 and King under check       []
