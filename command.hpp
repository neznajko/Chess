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
    int top_; // points to current position
    Move getmove( const std::string nput );
public:
    Command( Position& pos );
    enum { MOVE, UNDO, REDO, HELP, QUIT };
    int exec();
    void push();
    int back() const
    {
        return scoresheet_.size() - 1;
    }
};
////////////////////////////////////////////////////////
// log:
#endif// command_hpp
////////////////////////////////////////////////////////
