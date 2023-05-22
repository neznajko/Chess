////////////////////////////////////////////////////////
# ifndef ARMY_HPP
# define ARMY_HPP
////////////////////////////////////////////////////////
# include "Generator.hpp"
////////////////////////////////////////////////////////
//////////////////////////////////////////////////[Army]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
class Army {// dll( ring)
public:
    Unit * const king;
    std::vector<Unit *> bench; // out of play units
    Army( Unit * const king ): king( king ){
        king->prev = king->next = king;
    }
    ~Army();
    Unit * insert( const fig_t type,
                   const color_t color,
                   Node * const node );
    void unlink( Unit * const p, const bool del=false );
    Unit * dance();
};
////////////////////////////////////////////////////////
inline std::ostream&
operator<<( std::ostream& ostrm, const Army& army ){
    static const std::string name[]{
        "Black", "White"
    };
    ostrm << "<" << name[ army.king->GetColor()]
          << ">\n";
    Unit * unit{ army.king };
    do {
        ostrm << unit->gen;
        unit = unit->next;
    } while( unit != army.king );
    ostrm << army.bench << endl;
    return ostrm;
}
////////////////////////////////////////////////////////
# endif
////////////////////////////////////////////////////////
