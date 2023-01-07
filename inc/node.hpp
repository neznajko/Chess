////////////////////////////////////////////////////////////////
# ifndef NODE_HPP
# define NODE_HPP
////////////////////////////////////////////////////////////////
# include "move.hpp"
# include "cast.hpp"
////////////////////////////////////////////////////////////////
// + 0 1 2 3 4 5 6 7 8 9 a b c d e f                  i = k >> 4
// 0 = = = = = = = = = = = = = = = =                  j = k % 16
// 1 = = = = = = = = = = = = = = = =              k = i << 4 + j
// 2 = = 8 = r n b q k b n r = = = =
// 3 = = 7 = p p p p p p p p = = = =
// 4 = = 6 =                 = = = =
// 5 = = 5 =                 = = = =
// 6 = = 4 =                 = = = =
// 7 = = 3 =                 = = = =
// 8 = = 2 = P P P P P P P P = = = =
// 9 = = 1 = R N B Q K B N R = = = =   
// a = = = = a b c d e f g h = = = = 
// b = = = = = = = = = = = = = = = =
struct Board {
    static const int EXP = 4;
    static const int WIDTH = 1 << EXP;
    static const int DOJO_SIZE = 8;
    static const int FRAME_HEIGHT = 2;
    static const int FRAME_WIDTH = ( WIDTH - DOJO_SIZE )/2;
    static const int HEIGHT = DOJO_SIZE + 2*FRAME_HEIGHT;
    static const int SIZE = WIDTH * HEIGHT;
    enum{ NW, N, NE,
           W,     E,
          SW, S, SE, };
    static const int dR[];
    //
    static int geti( int k ){ return k >> EXP; }
    static int getj( int k ){ return k % WIDTH; }
    static int getk( int i, int j ){ return (i << EXP) + j; }
    static int getk( const std::string& sqr );
    static std::string coor( int k );
    ////////////////////////////////////////////////////////////
    // Convyort from normal npas def, vith respect to the active
    // side, vhich means that npas square belongs to the passive
    // side.
    static int npas_convyort( int npas, color_t acolor );
    //
    Unit* square[ SIZE ];
    //
    Board();
    Unit* getunit( int i, int j ) { // fen factory
        return square[ getk( i, j )];
    }
    Unit* getunit( int k ){
        return square[ k ];
    }
    void setunit( Unit* u, int k ){
        square[ k ] = u;
    }
    void setnil( int k ){
        static Unit* const NILPTR{(Unit*)&Unit::NIL};
        setunit( NILPTR, k );
    }
    void setgrd( int k ){
        static Unit* const GRDPTR{(Unit*)&Unit::GRD};
        setunit( GRDPTR, k );
    }
    void dump();
    Unit* liftoff( int k ){
        Unit* u{ getunit( k )};
        setnil( k );
        return u;
    }
    void land( Unit* u, int k ){
        setunit( u, k );
        u->k = k;
    }
};
////////////////////////////////////////////////////////////////
struct Army { // dll( ring)
    static const std::string NAME[ RED ];
    Unit* king;
    std::vector<Unit*> bench; // out of play units
    int cntrs[ Figure::NIL ] = {}; // figure counters
    //
    Army(): king( new Unit()) {
        king->prev = king->next = king;
    }
    Unit* insert( fig_t fig, int k );
    void unlink( Unit* p, const bool del=false );
    Unit* dance();
    ~Army();
};
////////////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& ostrm, const Army& army );
////////////////////////////////////////////////////////////////
struct Node {
    //
    bool flip_the_switch;
    Board brd[ RED ];
    Army army[ RED ];
    Cast cast = Cast( this );
    int npas = 0;
    std::vector<Move> move_history;
    std::vector<int>  npas_history;
    //
    color_t GetActvColor() const { return( color_t ) flip_the_switch; }
    color_t GetPasvColor() const { return( color_t )!flip_the_switch; }
    Board& actv(){ return brd[ GetActvColor() ]; }
    Board& pasv(){ return brd[ GetPasvColor() ]; }
    Unit* getActvUnit( int k ){ return actv().getunit( k ); }
    Unit* getPasvUnit( int k ){ return pasv().getunit( k ); }
    void paint();
    void dump();
    void setunit( color_t color, fig_t fig, int k );
    void do_the_npas( int sorc, int dest );
    void undo_the_npas( int dest, int sorc );
    void make_move( const Move& mv );
    void unmake_move();
    // remoo enemy piece at k, both from army and board
    void re_capture( int k );
    void re_uncapture();
    // mv active side piece at sorc square to dest
    void travel( int sorc, int dest );
    void untravel( int dest, int sorc );
};
///////////////////////////////////////////////////////////////=
struct FEN {
    static fig_t getfig( char c );
    static Node* cons( const std::string& fen );
};
////////////////////////////////////////////////////////////////
# endif//NODE_HPP
///////////////////////////////////////////////////////////////_
// log:
