#ifndef Thumbnail_h
#define Thumbnail_h
#endif /* Thumbnail_h */

#include "ofMain.h"

#define NAME_HEIGHT 10

class Thumbnail {
    ofEvent<ofVec2f> clickedInside;
    
    float width, height;
    
    ofImage image;
    string videoName;
    
    ofColor textColor;
    
public:
    Thumbnail(float w, float h, ofImage img, string name, ofColor txtColor);
    void draw(float x, float y);
};
