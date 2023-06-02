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
    if( 1 ){
        benchmark( perft, 1 );
    } else {
        const std::string fen{
//            "r4rk1/8/8/8/8/8/6B1/R3K2R b K - 0 1"
 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
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
///////////////////////////////////////////////////////=
// log:
// startpos
// perft 1 - 20      | 20      |
//       2 - 400     | 400     |
//       3 - 8902    | 8902    |
//       4 - 197281  | 197281  |
//       5 - 4865609 | 4865609 | 4.29256 sec
