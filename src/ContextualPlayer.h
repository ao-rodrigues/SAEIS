#ifndef ContextualPlayer_h
#define ContextualPlayer_h


#endif /* ContextualPlayer_h */

#include "ofMain.h"
#include "ofxXmlSettings.h"

class ContextualPlayer {
    const int PLAYER_WIDTH = 480;
    const int PLAYER_HEIGHT = 270;
    
    const int CAMERA_WIDTH = 480;
    const int CAMERA_HEIGHT = 270;
    
    ofxXmlSettings behavioursXML;
    ofVideoGrabber camera;
    
    ofVideoPlayer player;
    
    float videoAreaXPos, videoAreaYPos;
    float videoAreaWidth, videoAreaHeight;
    
    float videoXPos, videoYPos;
    float videoWidth, videoHeight;
    
    
    bool isFullscreen;
    
public:
    ContextualPlayer(string behavioursXml);
    
    void draw();
    void update();
    
    void setFullscreen(bool setFullscreen);
        
private:
    void loadVideo(const string path);
    void calculatePlayerDimensions();
    
};
