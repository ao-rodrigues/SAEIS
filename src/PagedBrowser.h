#ifndef Paged_Browser_h
#define Paged_Browser_h
#endif /* Paged_Browser_h */

#pragma once

#include "ofMain.h"

template <class T>
class PagedBrowser {
    float width;
    float height;
    float elementW;
    float elementH;
    
    std::vector<std::unique_ptr<T>> elements;
    
public:
    Paged_Browser(float w, float h, float elW, float elH, std::vector<std::unique_ptr<T>> els);
}
