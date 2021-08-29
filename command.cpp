////////////////////////////////////////////////////////
#include "command.hpp"
Command::Command( Position& pos ): _pos( pos), top_( -1)
{
    std::cerr << "Check your keyboard.\n";
    push();
}
////////////////////////////////////////////////////////
// e4d3-ah-pu-sa - on pa so
// c7c6          - capture, recapture or not?
// 0-0           - dzero x 2
// 0-0-0         - Castles on the Queen Side! 
// c2d1=Q        - promotion to Queen
Move Command::getmove( std::string nput )
{
    const int n{ static_cast<int>( nput.size() )};
    _type = 0; // moo
    if( nput[2] == '0' ) // Castles
    {   //
        const bool active_color{ _pos.get_active_color() };
        const std::string rank{ "81"[ active_color ]};
        _sors = "e" + rank;
        _dest = (( n > 3 ) ? "c" : "g" ) + rank;
        _type = CAST;
        return Move( Coor( _sors ), Coor( _dest ), _type );
    }
    _sors = nput.substr( 0, 2 );
    _dest = nput.substr( 2, 2 );
    // a7a8=R
    // c7d8=Q
    // e5d6-uu-pa-so
    // d4c3-oò-pu-sa
    if( n > 4 ) // promotion or oh-pu-so
    {
        if( nput[4] == '=' ) {
            const int c{ tolower( nput[5] )};
            _type |= std::string( "=qrbn" ).find( c );
        } else {
            _type = CRON | NPAS;
            return Move( Coor( _sors ), Coor( _dest ), _type );
        }
    }
    const Coor dest{ Coor( _dest )};
    if( _pos.get_board().abs_offset( dest ) != EMPTY )
    {
        _type |= CRON;
    }
    return Move( Coor( _sors ), Coor( _dest ), _type );
}
int Command::exec()
{
    std::string nput;
    std::cin >> nput;
    if( nput == "help") {
        std::cout << " === The Legend of Chess:\n"
            "a2a4          - move, capture, recapture or not?\n"
            "0-0           - Castles on the King side\n"
            "0-0-0         - Castles on the Queen side\n"
            "h7g8=R        - Promotion 0%\n"
            "b4a3-ah-pu-sò - oh-pa-so as well\n"
            "undo          - yea\n"
            "redo          - nop\n"
            "help          - Print thus!\n"
            ".             - quit\n";
        return HELP;
    } else if( nput == "undo" ){
        if( top_ > 0 ){ // if first can't undo
            _pos = scoresheet_[ --top_ ];
        }
        return UNDO;
    } else if( nput == "redo" ){
        if( top_ < back()){ // if last can't redo
            _pos = scoresheet_[ ++top_ ];
        }
        return REDO;
    } else if( nput == "." ){
        return QUIT;
    } else { //
        _pos.makemove( getmove( nput ));
        push();
    }
    return MOVE;
}
void Command::push()
{
    if( top_++ < back()){ 
        scoresheet_[ top_] = _pos; // override
    } else {
        scoresheet_.push_back( _pos);
    }
}
////////////////////////////////////////////////////////
// log: - Knight bee too dee two!
//      - Ooh aah no wow waw ooh woo!
// - rewrite getmove                                 [x]
// - The guy who has invented iterators shold be shot as
// well!                                             [v]
