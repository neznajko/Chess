////////////////////////////////////////////////////////
# include "Generator.hpp"
# include "Brush.hpp"
////////////////////////////////////////////////////////
////////////////////////////////////////////////////[SQ]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void SQ::Notify() const {
    // Here the Casl Generator is unsubscribing on King
    // and Rook moves that's why loop over copY.
    auto copY{ subscribers };
    for( Generator * const gen: copY ){
        gen->Update( offset );
    }
}
////////////////////////////////////////////////////////
Unit * SQ::SetUnit( Unit * const nextUnit ){
    Unit * const prevUnit{ this->unit };
    this->unit = nextUnit;
    if( this->unit ){
        this->unit->offset = offset;
    }
    Notify();
    return prevUnit;
}
////////////////////////////////////////////////////////
bool SQ::Check( const color_t color ) const {
    for( Generator const * const gen: subscribers ){
        if( gen->unit->GetColor() == color ){
            return true;
        }
    }
    return false;
}
////////////////////////////////////////////////////////
std::ostream&
operator<<( std::ostream& os, const SQ& sq ){
    static const Brush brush( "128;0;0", "0;0;0" );
    if( sq.subscribers.empty()){
        return os;
    }
    os << brush.patron( Board::GetCoord( sq.offset ))
       << " ";
    for( Generator const * const gen: sq.subscribers ){
        os << gen->unit << " ";
    }
    return os;
}
////////////////////////////////////////////////////////
