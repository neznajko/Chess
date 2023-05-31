////////////////////////////////////////////////////////
# ifndef BRUSH_HPP
# define BRUSH_HPP
////////////////////////////////////////////////////////
# include <sstream>
////////////////////////////////////////////////////////
class Board;
////////////////////////////////////////////////////////
///////////////////////////////////////////////[Painter]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
class Painter {
public:
    static std::stringstream draw(  const Board& board );
    static std::stringstream paint( const Board& board );
};
////////////////////////////////////////////////////////
/////////////////////////////////////////////////[Brush]
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
class Brush {
    const std::string prefix;
public:
    Brush( const std::string& f, const std::string& b );
    std::string patron( const std::string& txt ) const;
};
////////////////////////////////////////////////////////
# endif
////////////////////////////////////////////////////////
