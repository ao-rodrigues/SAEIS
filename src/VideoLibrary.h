#pragma once

#include "ofMain.h"
#include "Player.h"
#include "VideoBrowser.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"
#include "ofxOpenCv.h"

class VideoLibrary {
    struct DiffFrame {
        int absDiff;
        int originalIdx;
        ofTexture tex;
    };
    
    
    const string dir_path = "videos/";
    const string video_ext = ".mov";
    const string metadataFile = "metadata/metadata.xml";
    const string metadataTag = "METADATA";
    const string lastProcessedTag = "LAST-PROCESSED";
    const string previewFramesPath = "micons/";
    
    const int NUM_PREVIEW_FRAMES = 10;
    
    const int frameStep = 2;
    
    int frameStepCounter;
    
    string statusMsg;
    
    const float PADDING = 50.0;
    
    //int lastTagNum;
    
    ofDirectory dir;
    //std::vector<string> videoPaths;
    vector<string> videoNames;
    vector<DiffFrame> diffs;
    
    shared_ptr<Player> player;
    unique_ptr<VideoBrowser> videoBrowser;
    
    ofxInputField<string> tags;
    
    ofxXmlSettings XML;
    
    int lastProcessedIdx;
    
    bool videosProcessed;
    
    pair<ofxCvColorImage, ofxCvColorImage> framePair;
    bool learnFirstFrame, learnSecFrame;
    
    ofVideoPlayer hiddenPlayer;

    
    ofxCvGrayscaleImage gray;
    float percent;
    
public:
    VideoLibrary();
    void update();
    void draw();
    void key_pressed(int key);
    string extractVideoName(string path);
    
protected:
    void processVideos();
    void processFrames(ofxCvColorImage first, ofxCvColorImage second);
    void getPreviewFrames(string path, string name);
    
    int compareHistograms(vector<int> first, vector<int> second);
    
    static bool absDiffsCompare(const DiffFrame &i, const DiffFrame &j);
    static bool originalIdxCompare(const DiffFrame &i, const DiffFrame &j);
};

