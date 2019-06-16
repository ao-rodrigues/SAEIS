#pragma once

#include "ofMain.h"
#include "Player.h"
#include "VideoBrowser.h"
#include "ContextualPlayer.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"
#include "ofxOpenCv.h"

class VideoLibrary {
    struct DiffFrame {
        int absDiff;
        int originalIdx;
        ofTexture tex;
    };
    
    const string BEHAVIOURS_XML = "behaviours/behaviours_1.xml";
    
    const string IMG_REF = "feature_matching/cocacola.jpg";
    const string MATCHER = "FlannBased";
    
    const float MATCH_RATIO_THRESH = 0.75f;
    const int MIN_MATCH_COUNT = 10;
    
    const string NO_MATCH = "No match";
    
    const string DIR_PATH = "videos/";
    const string METADATA_FILE = "metadata/metadata.xml";
    
    const string METADATA_TAG = "METADATA";
    const string LAST_PROCESSED_TAG = "LAST-PROCESSED";
    const string PREVIEW_FRAMES_PATH = "micons/";
    const string HAAR_CASCADE_XML_PATH = "haar_cascades/haarcascade_frontalface_default.xml";
    
    const string PROCESSING_VIDEOS_MSG = "Processing video ";
    const string VIDEOS_PROCESSED_MSG = "Video processing done!";
    
    const string LOW_RYTHM = "LOW";
    const string MED_RYTHM = "MEDIUM";
    const string HIGH_RYTHM = "HIGH";
    
    const int NUM_PREVIEW_FRAMES = 10;
    
    const int frameStep = 5;
    
    const int K_SIZE = 31;
    const double SIGMA = 4.0;
    const double GAMMA = 0.5;
    
    const vector<double> THETA_VALUES = {0.0, 22.5, 45.0, 90.0, 112.5, 135.0};
    const vector<double> LAMBDA_VALUES = {8.0, 9.0, 10.0, 11.0};
    
    int frameStepCounter;
    
    string statusMsg;
    
    ofDirectory dir;
    vector<string> videoNames;
    vector<DiffFrame> diffs;
    
    shared_ptr<Player> player;
    unique_ptr<VideoBrowser> videoBrowser;
    unique_ptr<ContextualPlayer> contextualPlayer;
    
    bool fullscreenMode;
    
    ofxInputField<string> tagSearchInput;
    
    ofxXmlSettings XML;
    
    int lastProcessedIdx;
    
    bool videosProcessed;
    bool firstMomentProcessing;
    
    float sumAvgLuminances;
    int sumNumFaces;
    int sumVerPixels, sumHorPixels, sumDia45Pixels, sumDia135Pixels;
    
    int objMatchCount;
    
    pair<ofxCvColorImage, ofxCvColorImage> framePair;
    bool learnFirstFrame, learnSecFrame;
    
    ofVideoPlayer hiddenPlayer;
    

public:
    VideoLibrary();
    void update();
    void draw();
    void key_pressed(int key);
    
protected:
    void processVideos();
    void getColorFirstMoment(ofxCvColorImage frame);
    void processFrames(ofxCvColorImage first, ofxCvColorImage second);
    void getPreviewFrames(string path, string name);
    
    int compareHistograms(vector<int> first, vector<int> second);
    
    void processEdgeDistribution(ofxCvColorImage colorImg);
    
    void featureExtraction(ofxCvColorImage colorImg);
    
    void processTexture(ofxCvColorImage colorImg);
    
    int calculateRythm();
    
    static bool absDiffsCompare(const DiffFrame &i, const DiffFrame &j);
    static bool originalIdxCompare(const DiffFrame &i, const DiffFrame &j);
    
    void tagSearchInputSubmit(string & input);
    
};

