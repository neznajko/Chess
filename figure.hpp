                           ////
////////////////////////////////////////////////////////
# if !defined figure_hpp
# define      figure_hpp
# include "move.hpp"
# include <list>
////////////////////////////////////////////////////////
constexpr int NOFIGS{ 6}; // No. figures
typedef std::list<Move> moz_t;
//
# define BLACK 0 // Recently have read that its not good
# define WHITE 1 // to use define in C++ because it will
# define RED   2 // be whatever, but I'll take the risk.
////////////////////////////////////////////////////////
class Figure {             
protected:                
    static const int         _VALUES[];  // p=4, R=20, ..
    static const int         _AGILITY[]; // kQRpBN
    static const std::string _PIX[][ NOFIGS]; // [±], ..
    static const std::string _NAMES[]; // Knight, Pawn, ..
    int8_t _type; // KING, QUEEN, ..
    Coor   _coor; // 
    bool   _color;
    Figure( const int8_t type,
            const Coor& coor,
            const bool color):
        _type( type), _coor( coor), _color( color) {}
public:
    static const std::string _TRAITS[];  // k, K, b, ..
    enum { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN };
    moz_t moz;
    struct Node*  _node; // declaration of Node
    struct Board* brd_;  // shortcut
    //
    static Figure* factory( const char c, const Coor& coor);
    const Coor& coor() const { return _coor; }
    void set_coor( const Coor& coor) { _coor = coor; }
    int agility() const { return _AGILITY[ _type]; } 
    int value() const { return _VALUES[ _type]; }
    int type() const { return _type; }
    bool color() const { return _color; }
    char traits() const {
        return _TRAITS[ _color][ _type];
    }
    std::string pix() const {
        return _PIX[ _color][ _type];
    }
    virtual void getmoz() = 0; // moves
    virtual ~Figure() {};
    void setNode( Node* node);
    std::string mozstr() const;
    void ranger_moz( const int start, const int step);
};
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& ostrm, const Figure& f);
// 4 sorting
bool operator<( const Figure& f, const Figure& g);
////////////////////////////////////////////////////////
struct Unit {
    static const Unit* ZILCH;
    static const Unit* GUARD;
    Figure* f;
    Unit*   prev;
    Unit*   next;
    Unit( Figure* f=nullptr): f( f) {}
    ~Unit() { delete f; }
};
////////////////////////////////////////////////////////
class Knight: public Figure {
public:
    Knight( const Coor& coor, const bool color):
        Figure( KNIGHT, coor, color) {}
    void getmoz();
    ~Knight() {};
};
////////////////////////////////////////////////////////
class Bishop: public Figure {
public:
    Bishop( const Coor& coor, const bool color):
        Figure( BISHOP, coor, color) {}
    void getmoz();
    ~Bishop() {};
};
////////////////////////////////////////////////////////
class Pawn: public Figure {
public:
    Pawn( const Coor& coor, const bool color):
        Figure( PAWN, coor, color) {}
    void getmoz();
    void pmot_blow();
    ~Pawn() {};
};
////////////////////////////////////////////////////////
class Rook: public Figure {
public:
    Rook( const Coor& coor, const bool color):
        Figure( ROOK, coor, color) {}
    void getmoz();
    ~Rook() {};
};
////////////////////////////////////////////////////////
class Queen: public Figure {
public:
    Queen( const Coor& coor, const bool color):
        Figure( QUEEN, coor, color) {}
    void getmoz();
    ~Queen() {};
};
////////////////////////////////////////////////////////
class King: public Figure {
public:
    King( const Coor& coor, const bool color):
        Figure( KING, coor, color) {}
    void getmoz();
    bool undafire(); // Is in check?
    bool shortrange( const Coor* const dr,
                     const int type) const;
    bool longrange( const int start,
                    const int type) const;
    bool poke(); // Hit by an enemy pawn?
    // King's surrounding sqares; if set to false than
    // the King can't be there.
    bool aura[ Coor::WORLD ] = { true };
    void chck_aura();
    ~King() {};
};
////////////////////////////////////////////////////////
// log:
# endif ////////////////////////////////////////////////
    ////                                        ////
