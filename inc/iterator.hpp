////////////////////////////////////////////////////////////////
# ifndef ITERATOR_HPP
# define ITERATOR_HPP
///////////////////////////////////////////////////////////////_
# include "node.hpp"
////////////////////////////////////////////////////////////////
class Iterator {
protected:
    int          j_; // direction counter
    int       sorc_; // set by reset method
    int       cure_; // the current square, incrementing it in a
    // given direction will give the next destination square
    const int noof_; // number of directions
    Node*     node_;
public:  
    //==--__``
    Iterator( int noof, Node* node ):
        noof_( noof ), node_( node ) {}
    virtual ~Iterator() {};
    virtual Move get_next() = 0;
    virtual void reset( int sorc ){
        j_ = 0;
        cure_ = sorc_ = sorc;
    }
};
////////////////////////////////////////////////////////////////
std::ostream& operator<<( std::ostream& ostrm, Iterator* itor );
////////////////////////////////////////////////////////////////
class KingIterator: public Iterator {
private:
    int f_; // flank counter( the type is int cos of ++ )
public:
    KingIterator( Node* node ): Iterator( 8, node ) {}
    Move get_next();
    void reset( int sorc );
};
///////////////////////////////////////////////////////////////_
class KnightIterator: public Iterator {
public:
    KnightIterator( Node* node ): Iterator( 8, node ) {}
    Move get_next();
};
////////////////////////////////////////////////////////////////
// The code for Queen, Rook and Bishop is the same.
class LongRangeIterator: public Iterator {
private:
    // Number of directions for Queen, Rook and Bishop.
    static const int UNITS = 3;
    static int const _NOOF[];
    static const int* dP[ UNITS + 1 ];
    const int* dR;
public:
    LongRangeIterator( fig_t fig, Node* node ):
        Iterator( _NOOF[ fig ], node ), dR( dP[ fig ])
    {}
    Move get_next();
};
////////////////////////////////////////////////////////////////
# endif//ITERATOR_HPP
////////////////////////////////////////////////////////////////
