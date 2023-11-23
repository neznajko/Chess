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
    offset_t      offset{}; // Board offset
    fig_t         fig;      // can change with promotn
public:
    static Unit* const NIL; // empty square
    static Unit* const NOP; // guard square
    Unit( const fig_t fig,
          const color_t color,
          Node* const node );
    ~Unit();
    Node* const node; // backdoor 
    Generator* gen{}; // as fig can change with promotn
    Unit* prev;
    Unit* next;
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
    friend SQ; // allow access to private data
    friend std::ostream& operator<<
	( std::ostream& os, const Unit* const unit );
};
////////////////////////////////////////////////////////
inline std::ostream&
operator<<( std::ostream& os, const Unit* const unit ){
    return os << unit->GetStr()
              << Board::GetCoord( unit->offset );
}
//////_/////////////////////////////////////////////////
# endif
////////////////////////////////////////////////////////
