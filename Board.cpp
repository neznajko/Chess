////////////////////////////////////////////////////////
# include "Generator.hpp"
/////////////////////////////////////////////////[Board]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
offset_t Board::GetOffset( const std::string& sqr ){
    const char file{ sqr[ 0 ]}; // e
    const char rank{ sqr[ 1 ]}; // 4
    const int j{ file - 'a' + FRAME_WIDTH };
    const int i{ PMOT_ROW[ BLACK ] - ( rank - '1' )};
    return GetOffset( i, j );
}
////////////////////////////////////////////////////////
Board::Board() {
    static const int MASK[] = {
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,0,0,0,0,0,0,0,0,1,
        1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,        
    };
    for( offset_t j{}; j < SIZE; j++ ){
        auto unit{ MASK[ j ] ? Unit::NOP : Unit::NIL };
        sqs.push_back( SQ( j, unit ));
    }
}
////////////////////////////////////////////////////////
Unit * Board::Land( Unit * const unit,
                    const offset_t k ){
    Unit * prev{ sqs[ k ].SetUnit( unit )};
    unit->gen->Subscribe();
    return prev;
}
////////////////////////////////////////////////////////
Unit * Board::LiftOff( const offset_t k ){
    Unit * unit{ sqs[ k ].SetUnit( Unit::NIL )};
    unit->gen->Unsubscribe();
    return unit;
}
////////////////////////////////////////////////////////
Unit * Board::Travel( const offset_t src,
                      const offset_t dst ){
    Unit * const unit{ LiftOff( src )};
    return Land( unit, dst );
}
////////////////////////////////////////////////////////
void Board::Register( Generator * const gen,
                      const offset_t offset ){
    sqs[ offset ].Register( gen );
    gen->Insert( offset );
}
////////////////////////////////////////////////////////
void Board::Unregister( Generator * const gen,
                        const offset_t offset ){
    sqs[ offset ].Unregister( gen );
    gen->Erase( offset );
}
////////////////////////////////////////////////////////
void Board::InLoopUnregister
( Generator * const gen, const offset_t offset ){
    sqs[ offset ].Unregister( gen );
}
////////////////////////////////////////////////////////
color_t Board::GetUnitColor( const offset_t k ) const {
    Unit const * const unit{ GetUnit( k )};
    if( unit->isNIL()){
        return RED;
    }
    if( unit->isNOP()){
        return BLUE;
    }
    return unit->GetColor();
}
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& os, const Board& board ){
    for( int i{ Board::PMOT_ROW[ WHITE ]};
         i <= Board:: PMOT_ROW[ BLACK ]; ++i ){
        for( int j{ Casl::ROOK_COL[ QSIDE ]};
             j <= Casl::ROOK_COL[ KSIDE ]; ++j ){
            const auto k{ Board::GetOffset( i, j )};
            os << board.sqs[ k ];
        }
    }
    return os << endl;
}
////////////////////////////////////////////////////////
