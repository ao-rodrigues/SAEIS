#include "helper_functions.h"

void helper_functions::split(const std::string & str, char c, std::set<std::string> & v) {
    std::string::size_type i = 0;
    std::string::size_type j = str.find(c);
    
    if(j == std::string::npos) {
        v.insert(str);
        return;
    }
    
    while(j != std::string::npos) {
        v.insert(str.substr(i, j - 1));
        i = ++j;
        j = str.find(c, j);
        
        if(j == std::string::npos) {
            v.insert(str.substr(i, str.length()));
        }
    }
}

std::string helper_functions::extractVideoName(std::string path) {
    std::string baseFilename = path.substr(path.find_last_of("/\\") + 1);
    std::size_t extStart = baseFilename.find_last_of(".");
    
    return baseFilename.substr(0, extStart);
}

