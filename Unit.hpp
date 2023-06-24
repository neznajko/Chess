////////////////////////////////////////////////////////
# ifndef UNIT_HPP
# define UNIT_HPP
////////////////////////////////////////////////////////
# include "Board.hpp"
////////////////////////////////////////////////////////
//////////////////////////////////////////////////[Unit]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
class Node;
class Generator;
///////////////////////////////////////////////////////:
class Unit {
    const color_t color;
    fig_t fig; // will change with promotn
    offset_t offset{};
public:
    static Unit * const NIL; // empty square
    static Unit * const NOP; // guard square
    Unit( const fig_t fig,
          const color_t color,
          Node * const node );
    ~Unit();
    Node * const node; // backdoor pointer
    Generator * gen{}; // changing with promotn
    Unit * prev;
    Unit * next;
    bool isNIL() const {
        return this == NIL;
    }
    bool isNOP() const {
        return this == NOP;
    }
    color_t GetColor() const {
        return color;
    }
    fig_t GetFig() const {
        return fig;
    }
    offset_t GetOffset() const {
        return offset;
    }
    void SetFig( const fig_t fig );
    std::string GetStr() const;
    bool InCheck() const;
    //
    friend SQ;
    friend std::ostream&
    operator<<( std::ostream& os,
                Unit const * const unit );
};
////////////////////////////////////////////////////////
inline std::ostream&
operator<<( std::ostream& os, Unit const * const unit ){
    return os << unit->GetStr()
              << Board::GetCoord( unit->offset );
}
//////_/////////////////////////////////////////////////
# endif
////////////////////////////////////////////////////////
