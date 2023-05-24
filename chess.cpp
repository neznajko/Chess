///////////////////////////////////////////////////////>
# include <chrono>
///////////////////////////////////////////////////////_
# include "Com.hpp"
////////////////////////////////////////////////////////
void benchmark( void (*f)(), int NfLoops ){
    using namespace std::chrono;
    const auto start{ steady_clock::now()};
    while( NfLoops-- > 0 ){
        f();
    }
    const auto stop{ steady_clock::now()};
    duration<double> total{ stop - start };
    std::cout << total.count() << " sec\n";
}
///////////////////////////////////////////////////////_
int main(){
    if( 0 ){
    } else {
        const std::string fen{
            "r3kb1r/pppppppp/8/8/8/8/PPPPPPPP/R2QK2R w KQkq - 0 1"
        };
        Node * const node{ Node::cons( fen )};
        Com com{ node };
        com.Launch();
    }
}
////////////////////////////////////////////////////////
// +-------------------+ v, > - has
// | Node              | (n) - n items
// v-------v-(2)-------+ Node has a Board and 2 Armies..
// | Board | Army      |
// v-(120)-v-(>=1)-----+
// | SQ    > Unit      |
// |       v-----------+
// | (>=0) > Generator |
// +-------+-----------+
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
// DOTO: + install Stockfish
//       + Com::perft
//       + change SIGSEGV to SEGMENTATIONFAULT
//       - debug
//       + Pb7 is subscribed to a4
//       - Casl counters are not initialized properly.
//
