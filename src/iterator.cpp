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
// W  L   R  -Board::WIDTH
//      x    
// B  L   R  +Board::WIDTH
//
//   -1  +1
const int PawnIterator::FORK_DR[ RED ][ Nf_FORK_DR ] = {
    { +Board::WIDTH - 1, +Board::WIDTH + 1 }, // Black
    { -Board::WIDTH - 1, -Board::WIDTH + 1 }, // White
};
////////////////////////////////////////////////////////////////
const int PawnIterator::PMOT[] = {
    KNIGHT_PMOT,
    BISHOP_PMOT,
      ROOK_PMOT,
     QUEEN_PMOT
};
////////////////////////////////////////////////////////////////
const int PawnIterator::LAST_RANK[] = { 9, 2 };
const int PawnIterator::FYORST_RANK[] = { 3, 8 };
////////////////////////////////////////////////////////////////
Move PawnIterator::get_next() {
  ENTR:
    if(! pmot_stk.empty()){
        Move mv{ pmot_stk.back() };
        pmot_stk.pop_back(); 
        return mv;
    }
    while( j_ < noof_ ){
        int dest{ sorc_ + dF[ j_++ ]};
        Unit* p{ node_->getPasvUnit( dest )};
        if( p->isgrd()) continue;
        if( p->isnil()) {
            if( node_->npas != 0 ){
                auto acolor = node_->GetActvColor();
                int npas{ Board::npas_convyort( dest, acolor )};
                if( npas == node_->npas ){
                    return Move( CRON | NPAS, sorc_, npas );
                }
            }
            continue;
        }
        if( Board::geti( dest ) == LAST_RANK_ ){
            for( int p = 0; p < Nf_PMOT; ++p ){
                Move mv{ CRON | PMOT[ p ], sorc_, dest };
                pmot_stk.push_back( mv );
            }
            goto ENTR;
        }
        return Move( CRON, sorc_, dest );
    }
    while( i_ < kickoff ){
        int dest{ sorc_ + ( ++i_ ) * FORWARD_ };
        Unit* a{ node_->getActvUnit( dest )};
        Unit* p{ node_->getPasvUnit( dest )};
        if(!( a->isnil() and p->isnil())){
            return Move::Fin;        
        }
        if( Board::geti( dest ) == LAST_RANK_ ){
            pmot_stk.push_back( Move::Fin ); // et voilà
            for( int p = 0; p < Nf_PMOT; ++p ){
                Move mv{ MOVE | PMOT[ p ], sorc_, dest };
                pmot_stk.push_back( mv );
            }
            goto ENTR;
        }
        return Move( MOVE, sorc_, dest );
    }
    return Move::Fin;
}
void PawnIterator::reset( int sorc ){
    kickoff = Board::geti( sorc ) == FYORST_RANK_ ? 2 : 1;
    i_ = 0;
    Iterator::reset( sorc );
}
////////////////////////////////////////////////////////////////
//
//
