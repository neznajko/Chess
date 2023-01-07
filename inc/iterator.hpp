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
    void reset( int sorc ){
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
class RookIterator: public Iterator {
public:
    RookIterator( Node* node ): Iterator( 4, node ) {}
    Move get_next();
//    void reset( int sorc );
};
////////////////////////////////////////////////////////////////
class KnightIterator: public Iterator {
public:
    KnightIterator( Node* node ): Iterator( 8, node ) {}
    Move get_next();
//    void reset( int sorc );
};
////////////////////////////////////////////////////////////////
# endif//ITERATOR_HPP
////////////////////////////////////////////////////////////////
