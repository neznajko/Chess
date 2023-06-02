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
void perft(){
    const std::string fen{
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    };
    Node * const node{ Node::cons( fen )};
    node->Perft( 5 );
}
///////////////////////////////////////////////////////_
int main(){
    if( 0 ){
        benchmark( perft, 1 );
    } else {
        const std::string fen{
            "r3k2r/8/8/8/8/8/8/R3KB1R b Kk - 0 1"
// "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
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
// log:
// "r3k2r/8/8/8/8/8/8/R3KB1R b Kk - 0 1"
// perft 1 - 25      | 25
//       2 - 632     | 632
//       3 - 14940   | 14940
//       4 - 402028  | 400020
//       5 - 9638620 |
// stepback: Casl Counters are not updated properly.
// Make Update has two arguments square offset and previous
// piece color, long range units don|t need to update
// if cure/prev units are non nil.
