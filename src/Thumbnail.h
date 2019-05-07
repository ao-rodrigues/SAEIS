#ifndef Thumbnail_h
#define Thumbnail_h
#endif /* Thumbnail_h */

#include "ofMain.h"
#include "Player.h"

#define NAME_HEIGHT 10
#define BORDER_WIDTH 2

class Thumbnail{
    static const string THUMBNAILS_PATH;
    
    float xPos, yPos;
    float fixedXPos, fixedYPos;
    
    float width, height;
    float fixedW, fixedH;
    
    bool selected, enabled;
    
    shared_ptr<Player> player;
    
    //ofVideoPlayer videoPlayer;
    ofImage thumbnail;
    string videoPath, videoName;
    
    ofColor textColor;
    
    ofEventListener clickListener;
    
public:
    //ofEvent<const int> videoClicked;
    
    Thumbnail(float x, float y, float w, float h, string vidPath, shared_ptr<Player> plyr);
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
    void setSelected(bool isSelected);
    void setEnabled(bool isEnabled);
    string extractVideoName(string path);
    
};
