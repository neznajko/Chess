///////////////////////////////////////////////////////////////:
# include "iterator.hpp"
///////////////////////////////////////////////////////////////=
std::ostream& operator<<( std::ostream& ostrm, Iterator* itor ){
    while( true ){
        Move mv( itor->get_next());
        if( mv.sorc == -1 ){
            break;
        }
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
    static const int dR[] = {
        Board::dR[ Board::NW ], // Here the order doesn't matter
        Board::dR[ Board::N  ], // very much.
        Board::dR[ Board::NE ],
        Board::dR[ Board::E  ],
        Board::dR[ Board::W  ],
        Board::dR[ Board::SE ],
        Board::dR[ Board::S  ],
        Board::dR[ Board::SW ],
    };
  lab: //.. how to set a breakpoint here?
       // check this out: b src/iterator.cpp:29
    if( j_ >= noof_ ) return Move::Fin;
    int dest{ sorc_ + dR[ j_++ ]};
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
Move RookIterator::get_next() {
    static const int dR[] = {
        Board::dR[ Board::N ],
        Board::dR[ Board::E ],
        Board::dR[ Board::W ],
        Board::dR[ Board::S ],
    };
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
///////////////////////////////////////////////////////////////=
Move KnightIterator::get_next() {
    return Move::Fin;
}
///////////////////////////////////////////////////////////////_
