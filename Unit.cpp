////////////////////////////////////////////////////////
# include "Node.hpp"
//////////////////////////////////////////////////[Unit]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
Unit * const Unit::NIL{ nullptr };
Unit * const Unit::NOP{ NIL + 1 };
///////////////////////////////////////////////////////>
Unit::Unit( const fig_t fig,
            const color_t color,
            Node * const node ): color( color ),
                                 node( node ){
    SetFig( fig );
#   ifdef DEBUG
    std::cout << ">> " << this << endl;
#   endif
}
///////////////////////////////////////////////////////*
Unit::~Unit(){
    delete gen;
#   ifdef DEBUG
    std::cout << "<< " << this << endl;
#   endif    
}
///////////////////////////////////////////////////////_
void Unit::SetFig( const fig_t fig ){
    this->fig = fig;
    delete gen;
    gen = Generator::Factory( this );
}
////////////////////////////////////////////////////////
std::string Unit::GetStr() const {
    static const std::string STR[][ JOKER ]{
        { "k", "q", "r", "b", "n", "p" },
        { "K", "Q", "R", "B", "N", "P" },
    };
    if( isNIL()){
        return " ";
    } else if( isNOP()){
        return "?";
    } else {
        return STR[ color ][ fig ];
    }
}
////////////////////////////////////////////////////////
bool Unit::InCheck() const {
    return node->GetBoard().Check( offset, !color );
}
////////////////////////////////////////////////////////
