////////////////////////////////////////////////////////
#include "stuff.hpp"
#include "command.hpp"

#include <libgen.h> // basename
#include <unistd.h> // getopt

#include <fstream>
////////////////////////////////////////////////////////
namespace opt {
    std::string fen{
        "rnbqkbnr/pppppppp/8/8/"
        "8/8/PPPPPPPP/RNBQKBNR"
        " w KQkq - 0 0"
    };
    std::string path{}; // if empty no puzzles
    int start{ 1};      //
    int len{ 2};        //
    bool no_engine{ false };
    bool debug{ false };
    //    
    void usage( char* prog, const int code)
    {
        std::cout << basename( prog) 
                  << " -n (-m PATH) (-s START) (-l LEN) -h FEN\n"
            "\t-n\t\tno engine\n"
            "\t-m PATH\t\tpuzzle solvings\n"
            "\t-s START\tstarting puzzle\n"
            "\t-l LEN\t\tnumber of puzzles\n"
            "\t-h \t\tprint this\n\n"
            "\tFEN\t\tinitial position\n\n"
            " All fields are optional. For example after running\n"
            " \"./chess\" you will be able to play a game against\n"
            " the machine mua-ha-ha-ha-ha..\n";
        exit( code);
    }
    void getopt( int argc, char* argv[] )
    {
        int opt;
        while( (opt = ::getopt( argc, argv, "dnm:s:l:h")) != -1)
        {
            switch( opt) {
                case 'd': // undocumented!?
                    debug = true;
                    break;
                case 'n':
                    no_engine = true;
                    break;
                case 'm':
                    path = optarg;
                    break;
                case 's':
                    start = atoi( optarg);
                    break;
                case 'l':
                    len = atoi( optarg);
                    break;
                case 'h':
                default: usage( argv[0], -1);
            }
        }
        if( argc > optind) fen = argv[ optind];
    }
}
////////////////////////////////////////////////////////
class Game {
private:
    const std::string _fen;
public:
    Game( const std::string& fen): _fen( fen) {} // cons
    void view() const;
    void play() const;
};
void Game::view() const
{
    Position pos( _fen);
    Command com( pos);
    do {
        pos.spit_moves();
        pos.ordeer();
        std::cout << pos.get_active_color()
                  << ": "
                  << pos.hash()
                  << " ( " << pos.eval()
                  <<")\n";
        std::cout << pos, "$ ";
    } while( com.exec() != Command::QUIT);
}
void Game::play() const
{
    static const int depth{ 4}; // ply
    static const std::vector<std::string> msg = {
        "Nice mov!", "Uh..", "[ Ok]", "z-z-z-z",
        "Are you sure?", "o_o", "x x", "thats?",
        "?!", "poow", "ha-ha-ha",
    };
    Position pos( _fen);
    Command com( pos); // takes pos as reference
    do{
        // Anton to play
        // Display position and ask for a move.
        std::cout << pos, "$ ";
        // Get the command and figure what to do..
        switch( com.exec())
        {
            case Command::QUIT: return;
            case Command::UNDO:
            case Command::REDO:
            case Command::HELP: continue;
            default           : break;
        }
        // Now Bobby( The Computer) to mov here:
        int j = time( NULL) % msg.size();
        std::cout << pos << "$ " << msg[ j] << "\n" 
                  << std::flush;
        const auto scores{ pos.getScores( depth)};
        if( scores.empty()) {
            std::cout << "Checkmate!!\n";
            break;
        }
        const Move mov{ scores[ 0].move };
        pos.makemove( mov);
        com.push(); // write in scoresheet
    } while( true);
}
////////////////////////////////////////////////////////
std::vector<std::string> loadfen()
{
    std::ifstream ifs( opt::path);
    std::vector<std::string> vec;
    // j/start are 0/1 based
    int start{ opt::start - 1}; 
    const int finish{ start + opt::len};
    std::string buf;
    for( int j{}; j != finish; j++) {
        std::getline( ifs, buf);
        if( j < start) continue;
        vec.push_back( std::move( buf));
    }
    return vec;
}
////////////////////////////////////////////////////////
int main( int argc, char* argv[])
{
    setlocale( LC_CTYPE, "");
    opt::getopt( argc, argv);
    std::cout << opt::len << std::endl;
    
    //////////////////////////////////////////// tes ing
    if( opt::debug) {
        Position pos( opt::fen);
        pos.debug();
    }
    //return 0; ////////////////////////////////////t///
    if( opt::path.empty()) {
        Game game( opt::fen);
        if( opt::no_engine) {
            game.view();
        } else {
            game.play();
        }
    } else { // Puzzle time
        std::vector<std::string> vec{ loadfen()};
        for( const auto& fen: vec) {
            Game game( fen);
            game.view();
        }
    }
}
////////////////////////////////////////////////////////
// log: - New rules of chess, veneva a player makes an
// e.p. move is forced to say: o-o-o-o pa-a-a-a 
// so-o-o-o-o or some variant.
// 11:35
// - what's going on here?                           []
// - Segmentation fault on mate                      []
