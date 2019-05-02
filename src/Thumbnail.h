#ifndef Thumbnail_h
#define Thumbnail_h
#endif /* Thumbnail_h */

#include "ofMain.h"

#define NAME_HEIGHT 10
#define BORDER_WIDTH 5

class Thumbnail {
    float xPos, yPos;
    
    float width, height;
    
    ofImage image;
    string videoName;
    
    ofColor textColor;
    
    bool selected;
    
public:
    ofEvent<ofVec2f> clickedInside;
    
    Thumbnail(float x, float y, float w, float h, ofImage img, string name, ofColor txtColor);
    void draw();
    
    void mouseMoved(ofMouseEventArgs & args);
    void mouseDragged(ofMouseEventArgs & args);
    void mousePressed(ofMouseEventArgs & args);
    void mouseReleased(ofMouseEventArgs & args);
    void mouseScrolled(ofMouseEventArgs & args);
    void mouseEntered(ofMouseEventArgs & args);
    void mouseExited(ofMouseEventArgs & args);
    
    bool inside(float x, float y);
};
