////////////////////////////////////////////////////////
# include "Army.hpp"
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//////////////////////////////////////////////////[Army]
Unit * Army::insert( const fig_t type,
                     const color_t color,
                     Node * const node ){
    Unit * u{ king };
    if( type != KING ){
        u = new Unit( type, color, node );
        u->next = king->next;
        king->next = u;
        u->next->prev = u;
        u->prev = king;
    }
    return u;
}
////////////////////////////////////////////////////////
Army::~Army() {
    while( king->next != king ){
        unlink( king->next, true );
    }
    delete king;
    for( Unit * const p: bench ){
        delete p;
    }
}
////////////////////////////////////////////////////////
//   >next>    >next>                            :before
// n        p         q       
//   <prev<    <prev<
////////////////////////////////////////////////////////
//   >next>   ' bench:            >next>          :after
// n        q |        n        p        q
//   <prev<   |          <prev<
////////////////////////////////////////////////////////
void Army::unlink( Unit * const p, const bool del ){
    p->prev->next = p->next;
    p->next->prev = p->prev;
    if( del ){
        delete p;
    } else {
        bench.push_back( p );
    }
}
////////////////////////////////////////////////////////
Unit* Army::dance() {
    Unit* p{ bench.back() };
    bench.pop_back();
    p->prev->next = p->next->prev = p;
    return p;
}
////////////////////////////////////////////////////////
