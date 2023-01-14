///////////////////////////////////////////////////////////////:
# include "iterator.hpp"
///////////////////////////////////////////////////////////////=
std::ostream& operator<<( std::ostream& ostrm, Iterator* itor ){
    while( true ){
        Move mv( itor->get_next());
        if( mv.sorc == -1 ) break;
        ostrm << mv << " ";
    }
    return ostrm;
}
///////////////////////////////////////////////////////////////=
Move KingIterator::get_next() { //
    color_t acolor{ node_->GetActvColor() };
    // Check first the castle rytes, moving from QUEENSIDE to the
    // CENTER, incrementing alongside f_, the flank counter.
    static const int CASTYPE[] = { LONG_CAST, SHORT_CAST };
    //                             QUEENSIDE  KINGSIDE
    while( ++f_ < CENTER ){
        if( node_->cast.getrytes( acolor,( flank_t )( f_ ))){
            return Move( CASTYPE[ f_ ]);
        }
    }
  lab: //.. how to set a breakpoint here?
       // check this out: b src/iterator.cpp:29
    if( j_ >= noof_ ) return Move::Fin;
    int dest{ sorc_ + Board::dR[ j_++ ]};
    Unit* a{ node_->getActvUnit( dest )};
    if( a->isnil()) {
        Unit* p{ node_->getPasvUnit( dest )};
        int type = ( p->isnil()) ? MOVE : CRON;
        return Move( type, sorc_, dest );
    } else { // guard or friendly unit
        goto lab;
    }
}
void KingIterator::reset( int sorc ){
    f_ = QUEENSIDE - 1;
    Iterator::reset( sorc );
}
///////////////////////////////////////////////////////////////=
Move KnightIterator::get_next() {
    static const int dN[] = {
        -( Board::WIDTH << 1 ) - 1, // 0               - 0 - 1 -
        -( Board::WIDTH << 1 ) + 1, // 1               2 - - - 3
        -( Board::WIDTH      ) - 2, // 2               - - N - - 
        -( Board::WIDTH      ) + 2, // 3               4 - - - 5
         ( Board::WIDTH      ) - 2, // 4               - 6 - 7 -
         ( Board::WIDTH      ) + 2, // 5
         ( Board::WIDTH << 1 ) - 1, // 6
         ( Board::WIDTH << 1 ) + 1, // 7
    };
  lab:
    if( j_ >= noof_ ) return Move::Fin;
    int dest{ sorc_ + dN[ j_++ ]};
    Unit* a{ node_->getActvUnit( dest )};
    if( a->isnil()) {
        Unit* p{ node_->getPasvUnit( dest )};
        int type = ( p->isnil()) ? MOVE : CRON;
        return Move( type, sorc_, dest );
    } else { // guard or friendly unit
        goto lab;
    }
}
///////////////////////////////////////////////////////////////=
/////////////////////////////////////////////Q//R//B////////////
const int  LongRangeIterator::_NOOF[] = { 0, 8, 4, 4 };
const int* LongRangeIterator::dP[ LongRangeIterator::UNITS + 1 ] = {
    nullptr, Board::dR, Board::dROOK, Board::dBISHOP,
};
///////////////////////////////////////////////////////////////=
Move LongRangeIterator::get_next() {
  lab:
    if( j_ >= noof_ )return Move::Fin;
    int dest{ cure_ + dR[ j_ ]};
    Unit* a{ node_->getActvUnit( dest )};
    if( a->isnil()) {
        Unit* p{ node_->getPasvUnit( dest )};
        auto type{ p->isnil() ? MOVE : CRON };
        cure_ = dest;
        return Move( type, sorc_, dest );
    } else {
        ++j_;
        cure_ = sorc_;
        goto lab;
    } 
}
///////////////////////////////////////////////////////////////_
//
//
