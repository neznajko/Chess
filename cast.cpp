////////////////////////////////////////////////////////
# include "cast.hpp"
# include "figure.hpp" // WHITE, BLACK
////////////////////////////////////////////////////////
void Castles::setFenRytes( const std::string& fenRytes ){
    if( fenRytes[ 0 ] == '-' ){
        return;
    }
    for( const char c: fenRytes ){        
        switch( c )
        {
            case 'K': setRyte( WHITE, KINGSIDE ); break;
            case 'Q': setRyte( WHITE, QWENSIDE ); break;
            case 'k': setRyte( BLACK, KINGSIDE ); break;
            case 'q': setRyte( BLACK, QWENSIDE ); break;
            default:  std::cerr << "On Y Va!!,\n";
        }
    }
}
////////////////////////////////////////////////////////
