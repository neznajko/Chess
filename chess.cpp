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
//            "4k2r/8/8/8/8/8/8/4K2R b Kk - 0 1"
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
////////////////////////////////////////////////////////
// log: from start positoin
//      Perft 4 1.29796 sec ->  1.5947 sec
//      Perft 5 34.4201 sec -> 42.8857 sec
//      with InLoopUnregister  with copy 
//      + consider how to remove copY from SQ::Notify
//        in makemove copy rytes in the prologue and at
//        epilogue call UndoElevate or something and
//        call unsubsgribe from there
// Perft 4:  1.04 -> 0.138752 sec
// Perft 5: 28.01 -> 3.47675 sec
// Ok this is after -O3 optimization flag
// With optimization I get sigsegv after drawing the
// board. Check what is causing the problem.
//
