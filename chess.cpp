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
const std::string STARTPOS{
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
};
///////////////////////////////////////////////////////=
void perft(){
    const std::string fen{ STARTPOS };
    Node * const node{ Node::cons( fen )};
    node->Perft( 4 );
    delete node;
}
///////////////////////////////////////////////////////_
int main(){
    if( 1 ){
        benchmark( perft, 1 );
    } else {
        const std::string fen{
            STARTPOS
        };
        Node * const node{ Node::cons( fen )};
        std::cout << node->FEN() << endl
                  << node;
        delete node;
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
// log: startpos perft   ;
// 1 - 20      | 20      |
// 2 - 400     | 400     |
// 3 - 8902    | 8902    |
// 4 - 197281  | 197281  |
// 5 - 4865609 | 4865609 | 4.29256 sec
//                       | 3.57119 sec !!NWR
//                       | 1.89115 sec avec threads
//                       '
// - make NIL, NOP actual objects
// - move Node::MakeCasl to Casl::MakeCasl( node )
// - consider Node::King( c ) := army[ c ].king
// - try the improved perft with terminal depth=1
// - make thread pool with fixed size for Perft
// 
