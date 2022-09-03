////////////////////////////////////////////////////////
# include "node.hpp"
# include "cast.hpp"

# include <locale.h> // setlocale
# include <libgen.h> // basename
# include <unistd.h> // getopt

# include <cstdlib>  // atoi,exit

# include <chrono>
////////////////////////////////////////////////////////
namespace opt {
    std::string fen { "rnbqkbnr/"
                      "pppppppp/"
                      "8/8/8/8/"
                      "PPPPPPPP/"
                      "RNBQKBNR"
                      " w KQkq - 0 0" };
    std::string path{}; // if empty no puzzles
    int start{ 1};      //
    int len{ 2};        //
    bool no_engine{ false};
    bool debug{ false};
    void usage( char* prog, const int code) {
        std::cout << basename( prog) << 
            " -n (-m PATH) (-s START) (-l LEN) -h FEN\n"
            "\t-n\t\tno engine\n"
            "\t-m PATH\t\tpuzzle solvings\n"
            "\t-s START\tstarting puzzle\n"
            "\t-l LEN\t\tnumber of puzzles\n"
            "\t-h \t\tprint this\n\n"
            "\tFEN\t\tinitial position\n\n"
            " All fields are optional."
            " For example after running\n"
            " \"./chess\" you will be able"
            " to play a game against\n"
            " the machine mua-ha-ha-ha-ha..\n";
        exit( code);
    }
    void getopt( int argc, char* argv[]) {
        int opt;
        const char optstr[] = "dnm:s:l:h";
        while(( opt = ::getopt( argc, argv, optstr)) != -1) {
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
                    usage( argv[ 0], 0);
                default:
                    usage( argv[ 0], 1);
            }
        }
        if( argc > optind) fen = argv[ optind];
    }
}
////////////////////////////////////////////////////////
void trythus() {
    using namespace std::chrono;
    const auto start{ steady_clock::now()};
    // FEN position is taken from program's arguments.
    std::cout << opt::fen << endl;
    // Create a Node instance from it.
    Node node( opt::fen);
    // Dump the board position on stdout.
    std::cout << node.board;
    // Get the side to move.
    const bool active_color = node.active_color;
    std::cout << "( " << Army::NAME[ active_color ]
              << " to move )\n";
    // Print both armies
    for( bool color: { BLACK, WHITE }){
        std::cout << Army::NAME[ color] << ": "
                  << node.army[ color] << endl;
    }
    // Dump enemy counters
    for( int j = 0; j < NOFIGS; j++) {
        std::cout << Figure::_TRAITS[ !active_color][ j] << ' '
                  << node.army[ !active_color].cntrs[ j]
                  << endl;
    }
    switch( 0 ){
        case 0:
        {
            auto king = (King*)(node.army[ active_color].king->f);
            if( king ){
                king->getmoz();
                std::cout << king->mozstr() << endl;
            }
            break;
        }
    }
    //
    const auto stop{ steady_clock::now()};
    duration<double> total = stop - start;
    std::cout << total.count() << " sec\n";
}
////////////////////////////////////////////////////////
int main( int argc, char* argv[]) {
    setlocale( LC_CTYPE, "");
    opt::getopt( argc, argv);
    //////////////////////////////////////////// tes ing
    if( opt::debug) {
        trythus();
        return 0; //////////////////////////////////t///
    }
}
////////////////////////////////////////////////////////
// log: - revert() with bench as cron move stack     []
// - work on castles rights                          []
// - move object code in obj dir or smth             []
