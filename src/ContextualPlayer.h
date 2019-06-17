#ifndef ContextualPlayer_h
#define ContextualPlayer_h


#endif /* ContextualPlayer_h */

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxOpenCv.h"

class ContextualPlayer {
    const string IMG_REF = "feature_matching/cocacola.jpg";
    
    const string HAAR_CASCADE = "haar_cascades/haarcascade_frontalface_default.xml";
    
    const string EVT_NUM_FACES = "NUM-FACES";
    const string EVT_MOTION = "MOTION";
    
    
    const int PLAYER_WIDTH = 480;
    const int PLAYER_HEIGHT = 270;
    
    const int CAMERA_WIDTH = 640;
    const int CAMERA_HEIGHT = 480;
    
    const uint64_t VIDEO_PLAY_TIME_MILLIS = 5000;
    
    const int MIN_NUM_FACES = 2;
    
    ofxXmlSettings behavioursXML;
    ofVideoGrabber camera;
    
    ofVideoPlayer player;
    
    ofxCvHaarFinder faceFinder;
    
    float videoAreaXPos, videoAreaYPos;
    float videoAreaWidth, videoAreaHeight;
    
    float videoXPos, videoYPos;
    float videoWidth, videoHeight;
    
    int camX, camY;
    
    uint64_t videoStart;
    
    bool isFullscreen;
    
    bool objDetected;
    vector<cv::Point2f> objCoords;
    
public:
    ContextualPlayer(string behavioursXml);
    
    void draw();
    void update();
    
    void setFullscreen(bool setFullscreen);
        
private:
    void loadVideo(const string videoTag);
    void calculatePlayerDimensions();
    
    void processFrame(ofPixels &pixels);
    
    void detectObject(ofPixels &scenePixels);
    void setupObjectRect(vector<cv::Point2f> objCorners);
    
    string getRandomVideoPath(string tag);
};
