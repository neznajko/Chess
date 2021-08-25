////////////////////////////////////////////////////////
#if !defined command_hpp
#define command_hpp

#include <iostream>

#include "position.hpp"
////////////////////////////////////////////////////////
class Command {
private:
    Position& _pos;
    std::string _sors;
    std::string _dest;
    int8_t _type; // capture, recapture; what is this?
    std::vector<Position> scoresheet_; // stack
    int top_; // points to last drawn position
    Move getmove( const std::string nput );
public:
    Command( Position& pos );
    enum { MOVE, UNDO, REDO, HELP, QUIT };
    int exec();
};
////////////////////////////////////////////////////////
// log: - encapsulate Move                           [x]
//      - discard _sors and stuff                    []
#endif// command_hpp
////////////////////////////////////////////////////////
