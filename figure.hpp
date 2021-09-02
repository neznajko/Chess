////////////////////////////////////////////////////////
#if !defined figure_hpp
#define figure_hpp

#include "coor.hpp"

enum { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN };
enum { BLACK, WHITE, RED };
////////////////////////////////////////////////////////
class Figure {
private:
    int8_t _type; // complemented for Black
    Coor   _coor; // Negative if out of play
public:
    static const std::string _UNITS[];
    const static int _VALUES[];
    //
    Figure( const char c, const int i, const int j );
    bool  getColor() const { return !(_type < 0 ); }
    int  getUnit() const; // KING, KNIGHT, ..
    char getChar() const; // k, +, 2, ...
    const Coor& get_coor() const { return _coor; }
    void set_coor( const Coor& coor )
    {
        _coor = coor;
    }
    void putOutOfPlay()
    {
        _coor.set_i( -_coor.get_i() );
    }
    bool isOutOfPlay() const
    {
        return _coor.get_i() < 0;
    }
    void pmot( const int unit );
    friend std::ostream&
    operator<<( std::ostream& os, const Figure& fig );
    std::string getPix() const;
};
////////////////////////////////////////////////////////
// log: There is a lot of repetitions in units sequence
// "kqrbnp", I have to be able to reorder the units in
// one place, so that it will affect their sorting
// order in Position constructor for example
#endif// figure_hpp
////////////////////////////////////////////////////////
