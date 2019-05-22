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
    
    const string haarCascadeXmlPath = "haar_cascades/haarcascade_frontalface_default.xml";
    
    const string PROCESSING_VIDEOS_MSG = "Processing videos...";
    const string VIDEOS_PROCESSED_MSG = "Video processing done!";
    
    const string LOW_RYTHM = "LOW";
    const string MED_RYTHM = "MEDIUM";
    const string HIGH_RYTHM = "HIGH";
    
    const int NUM_PREVIEW_FRAMES = 10;
    
    const int frameStep = 5;
    
    int frameStepCounter;
    
    string statusMsg;
    
    ofDirectory dir;
    vector<string> videoNames;
    vector<DiffFrame> diffs;
    
    shared_ptr<Player> player;
    unique_ptr<VideoBrowser> videoBrowser;
    
    ofxInputField<string> tagSearchInput;
    
    ofxXmlSettings XML;
    
    int lastProcessedIdx;
    
    bool videosProcessed;
    bool colorProcessed;
    
    float sumAvgLuminances;
    int sumNumFaces;
    int sumVerPixels, sumHorPixels, sumDia45Pixels, sumDia135Pixels;
    
    pair<ofxCvColorImage, ofxCvColorImage> framePair;
    bool learnFirstFrame, learnSecFrame;
    
    ofVideoPlayer hiddenPlayer;

public:
    VideoLibrary();
    void update();
    void draw();
    void key_pressed(int key);
    string extractVideoName(string path);
    
protected:
    void processVideos();
    void getColorFirstMoment(ofxCvColorImage frame);
    void processFrames(ofxCvColorImage first, ofxCvColorImage second);
    void getPreviewFrames(string path, string name);
    
    int compareHistograms(vector<int> first, vector<int> second);
    
    void processEdgeDistribution(ofxCvColorImage colorImg);
    
    int calculateRythm();
    
    static bool absDiffsCompare(const DiffFrame &i, const DiffFrame &j);
    static bool originalIdxCompare(const DiffFrame &i, const DiffFrame &j);
    
    void tagSearchInputSubmit(string & input);
    
};

