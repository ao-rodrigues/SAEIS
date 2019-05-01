#pragma once

#include "ofMain.h"

#define INNER_PADDING_SIDES 20
#define ELEMENTS_DISTANCE_X 10
#define ELEMENTS_DISTANCE_Y 15

template <class T>
class PagedBrowser {
    float width;
    float height;
    float elementW;
    float elementH;
    
    int elementsPerLine;
    int elementsPerCol;
    int numPages;
    
    std::vector<std::unique_ptr<T>> elements;
    
    ofVec2f clickedPoint;
    
public:
    PagedBrowser(float w, float h, float elW, float elH, std::vector<std::unique_ptr<T>> els);
    void draw();
};
