#ifndef TextToggle_h
#define TextToggle_h
#endif /* TextToggle_h */

#include "ofMain.h"

class TextToggle{
    float xPos, yPos;
    float width, height;
    
    string text;
    bool toggled;
    
public:
    ofEvent<string> textClicked;
    TextToggle(float x, float y, float w, float h, string txt, bool startToggled);
    void draw();
    
    void mouseMoved(ofMouseEventArgs & args);
    void mouseDragged(ofMouseEventArgs & args);
    void mousePressed(ofMouseEventArgs & args);
    void mouseReleased(ofMouseEventArgs & args);
    void mouseScrolled(ofMouseEventArgs & args);
    void mouseEntered(ofMouseEventArgs & args);
    void mouseExited(ofMouseEventArgs & args);
    
    bool inside(float x, float y);
    
    void setToggled(bool isToggled);
    
    string getText();
};
