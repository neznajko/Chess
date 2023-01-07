////////////////////////////////////////////////////////
# include <locale.h> // setlocale
////////////////////////////////////////////////////////////////
# include <iostream>
# include <chrono>
# include <cstdlib>
////////////////////////////////////////////////////////////////
# include "com.hpp"
////////////////////////////////////////////////////////////////
void benchmark( void (*f)()) {
    using namespace std::chrono;
    const auto start{ steady_clock::now()};
    f();
    const auto stop{ steady_clock::now()};
    duration<double> total = stop - start;
    std::cout << total.count() << " sec\n";
}
////////////////////////////////////////////////////////////////
int main( int argc, char* argv[]) {
    setlocale( LC_CTYPE, "");
    if( 0 ){
    } else {
        const std::string fen = 
            ( argc > 1 ) ? argv[1] :
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w qkQK - 0 0"
            ;
        Node* node{ FEN::cons( fen )};
        Com com( node );
        do { node->dump(); } while( com.exec());
        delete node;
    }
}
////////////////////////////////////////////////////////////////
// log:
