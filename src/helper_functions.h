#ifndef helper_functions_h
#define helper_functions_h


#endif /* helper_functions_h */

#include <string>
#include <set>
#include <stdio.h>

namespace helper_functions {
    void split(const std::string & str, char c, std::set<std::string> & v);
    
    std::string extractVideoName(std::string path);
}
