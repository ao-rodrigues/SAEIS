#ifndef Thumbnail_h
#define Thumbnail_h
#endif /* Thumbnail_h */

#include "ofMain.h"

#define NAME_HEIGHT 10
#define BORDER_WIDTH 2

class Thumbnail{
    float xPos, yPos;
    float fixedXPos, fixedYPos;
    
    float width, height;
    float fixedW, fixedH;
    
    ofVideoPlayer videoPlayer;
    string videoPath;
    
    ofColor textColor;
    
    bool selected;
    
    ofEventListener clickListener;
    
public:
    //ofEvent<ofVideoPlayer> videoClicked;
    
    Thumbnail(float x, float y, float w, float h, string vidPath, ofColor txtColor);
    void draw();
    void update();
    
    void mouseMoved(ofMouseEventArgs & args);
    void mouseDragged(ofMouseEventArgs & args);
    void mousePressed(ofMouseEventArgs & args);
    void mouseReleased(ofMouseEventArgs & args);
    void mouseScrolled(ofMouseEventArgs & args);
    void mouseEntered(ofMouseEventArgs & args);
    void mouseExited(ofMouseEventArgs & args);
    
    
    void onMouseReleased(ofMouseEventArgs & args);
    bool inside(float x, float y);
    
};
