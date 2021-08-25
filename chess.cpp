////////////////////////////////////////////////////////
#include "stuff.hpp"
#include "command.hpp"
#include <libgen.h> // basename
#include <unistd.h> // getopt
#include <sstream>
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
        std::stringstream ss;
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
                    ss << optarg;
                    ss >> start;
                    break;
                case 'l':
                    ss << optarg;
                    ss >> len;
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
void Game::play() const
{
    static const int depth{ 4}; // ply
    Position pos( _fen);
    Command com( pos);
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
        std::cout << pos << "Thinking ..: " << std::flush;
        // Now Bobby( The Computer) to mov here:
        const Move mov{ pos.getScores( depth)[ 0].move };
        std::cout << mov.getSors(), ' ', mov.getDest(), '\n';
        pos.makemove( mov);
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
    //////////////////////////////////////////// tes ing
    if( opt::debug) {
        Position pos( opt::fen);
        std::cout << pos, '\n';
        for( const auto& q: pos.fork()) {
            
            q.debug_spit();
        }
        return 0;
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
// - debug
// - help command
//
// 11:35
// 13:36 - 13:50 = 14 min
// 14:14 - 14:43 = 29 min ( New Record !!)
// 15:24 - 16:37 = 1h 13 min ( o_o)
// 13:31 - 15:40 = 1h 09 min ( Nice try)
// 17:34 - 18:45 = 1h 11 min ( Close)
// 10:03 - 13:02 = 2h 59 min ( New Record !!)
// 15:03 -
