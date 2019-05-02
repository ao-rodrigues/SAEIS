#include <stdio.h>
#include <exception>

using namespace std;

class _InvalidDimensionsException : public exception {
    virtual const char* what() const throw() {
        return "Invalid dimensions!";
    }
    
}InvalidDimensionsException;
