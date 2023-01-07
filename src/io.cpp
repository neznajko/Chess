                          ////
////////////////////////////////////////////////////////
# include "io.hpp"
const std::string CSI{ "\033[" };
////////////////////////////////////////////////////////
// foreground, background
Brush::Brush( const std::string& f,
              const std::string& b ):
    prefix{ CSI + ";38;2;" + f + ";48;2;" + b + ";1m" }
{}
// thats the spit
std::string Brush::patron( const std::string& txt ) const
{
    static const std::string suffix{ CSI + "0m" };
    return( prefix + txt + suffix );
}
////////////////////////////////////////////////////////
// log:
////////////////////////////////////////////////////////
    ////                                        ////
