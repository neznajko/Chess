////////////////////////////////////////////////////////
#include "stuff.hpp"
#include "command.hpp"
////////////////////////////////////////////////////////
namespace opt {
    std::string fen;
    void getopt( int argc, char* argv[] )
    {
        if( argc > 1) {
            fen = argv[1];
        } else {
            fen = ( "rnbqkbnr/pppppppp/8/8/"
                    "8/8/PPPPPPPP/RNBQKBNR"
                    " w KQkq - 0 0" );
        }
    }        
}
////////////////////////////////////////////////////////
#include <cstdlib>
void playgame( const std::string fen)
{
    Position pos( fen);
    Command com( pos);
    do{
        pos.getScores( 4);
        std::cout << pos, "$ ";
        if( !com.exec()) break;
    } while( true);
 }
////////////////////////////////////////////////////////
// log: What if player has no mooz?                  []
#include <fstream>
std::vector<std::string>
loadfen( const std::string& path,
         int start = 1,
         const int len = 10)
{
    std::ifstream ifs( path);
    std::string s;
    std::vector<std::string> vec;
    //
    --start; // j/start are 0/1 based
    const int finish{ start + len};
    for( int j{}; j != finish; j++) {
        std::getline( ifs, s);
        if( j < start) continue;
        vec.push_back( std::move( s));
    }
    return vec;
}
////////////////////////////////////////////////////////
#include <ctime>
int main( int argc, char* argv[])
{
    setlocale( LC_CTYPE, "");
    opt::getopt( argc, argv);
    srand( time( NULL));
    //////////////////////////////////////////// tes ing
    //return 0; ////////////////////////////////////t///
    if( 1) {
        playgame( opt::fen);
    } else {
        std::vector<std::string>
            vec = loadfen( "./fen/mate2.txt", 25, 2);
        for( const auto& fen: vec) {
            Position pos( fen);
            Command com( pos);
            do{
                pos.spit_moves();
                pos.ordeer();
                std::cout << pos.get_active_color()
                          << ": "
                          << pos.hash()
                          << " ( " << pos.eval()
                          <<" )\n";
                std::cout << pos, "$ ";
            } while( com.exec());
        }
    }
    return 0;
}
////////////////////////////////////////////////////////
// log: - New rules of chess, veneva a player makes an
// e.p. move is forced to say: o-o-o-o pa-a-a-a 
// so-o-o-o-o or some variant.
// 11:35
// - Why I need depth 4 to find mate in 2? []
// - Upload and backup                     []
