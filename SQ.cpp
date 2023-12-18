//////////////////////////////////////////////////////////
# include "Generator.hpp"
# include "Brush.hpp"
////////////////////////////////////////////////////////
////////////////////////////////////////////////////[SQ]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void SQ::Notify() const {
    for( Generator* const gen: subscribers ){
        gen->Update( offset );
    }
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
    for( const Generator* const gen: sq.subscribers ){
        os << gen->unit << " ";
    }
    return os;
}
////////////////////////////////////////////////////////
Unit* SQ::SetUnit( Unit* const nextUnit ){
    Unit* const prevUnit{ this->unit };
    this->unit = nextUnit;
    if( this->unit ){
        this->unit->offset = offset;
    }
    // Notify only if nextUnit or prevUnit is nil
    if( prevUnit->isNIL() or nextUnit->isNIL()){
        Notify();
	}
    return prevUnit;
}
////////////////////////////////////////////////////////
bool SQ::Check( const color_t color ) const {
    for( const Generator* const gen: subscribers ){
        if( gen->unit->GetColor() == color ){
            return true;
        }
    }
    return false;
}
////////////////////////////////////////////////////////
// log: Consider generator being private to unit, make
// a bridge between gen an unit clients, subscribe units
// instead of generators.
