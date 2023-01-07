                          ////
////////////////////////////////////////////////////////
# ifndef IO_HPP
# define IO_HPP
////////////////////////////////////////////////////////
# include <string>
# include <iostream>
# include <vector>
////////////////////////////////////////////////////////
const char endl = '\n';
////////////////////////////////////////////////////////
class Brush {
private:
    const std::string prefix;
public:
    Brush( const std::string& f, const std::string& b );
    std::string patron( const std::string& txt ) const;
};
////////////////////////////////////////////////////////////////
template <typename T>
std::ostream& operator<<( std::ostream& ostrm,
                          const std::vector<T>& vec ){
    ostrm << "[";
    for( int j = 0; j < vec.size(); j++ ){
        ostrm << ","[ !j ] << vec[j];
    }
    return ostrm << "]";
}
////////////////////////////////////////////////////////////////
template <typename T>
std::ostream& operator<<( std::ostream& ostrm,
                          const std::vector<T*>& vec ){
    ostrm << "[";
    for( int j = 0; j < vec.size(); j++ ){
        ostrm << ","[ !j ] << *vec[j];
    }
    return ostrm << "]";
}
////////////////////////////////////////////////////////////////
# endif ////////////////////////////////////////////////
// log: Brush b( "230;120;13", "245;200;18");
//      std::cout << b.patron( " boom") << endl;
////////////////////////////////////////////////////////
    ////                                        ////    
