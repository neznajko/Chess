////////////////////////////////////////////////////////
#if !defined stuff_hpp
#define stuff_hpp

#include <ostream>
#include <string>
////////////////////////////////////////////////////////
template <typename T>
std::ostream& operator,( std::ostream& out, const T& x )
{
    return out << x;
}
////////////////////////////////////////////////////////
class Brush {
private:
    const std::string _prefix;
public:
    // foreground, background
    Brush( const std::string& f, const std::string& b ):
        _prefix( "\033[;38;2;" + f +
                 ";48;2;" + b + ";1m" ) {}
    // thats the spit
    std::string patron( const std::string& txt ) const
    {
        return _prefix + txt + "\033[0m";
    }
};
////////////////////////////////////////////////////////
// log:
#endif// stuff_hpp
////////////////////////////////////////////////////////
