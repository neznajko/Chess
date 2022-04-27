////////////////////////////////////////////////////////
# include "cast.hpp"
# include "figure.hpp"
////////////////////////////////////////////////////////
void Castles::setFenRytes( const std::string& fenRytes ){
    if( fenRytes[ 0 ] == '-' ){
        return;
    }
    for( const char c: fenRytes ){        
        switch( c )
        {
            case 'K': setRyte( WHITE, KINGSIDE ); break;
            case 'Q': setRyte( WHITE, QEENSIDE ); break;
            case 'k': setRyte( BLACK, KINGSIDE ); break;
            case 'q': setRyte( BLACK, QEENSIDE ); break;
            default:  std::cerr << "On Y Va!!,\n";
        }
    }
}
////////////////////////////////////////////////////////
