#pragma once

#include "ofMain.h"
#include "Thumbnail.h"
#include "TextToggle.h"
#include "Player.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"

#define SPACING_X 10
#define SPACING_Y 10
#define PAGE_NUMS_HEIGHT 15
#define INDIVIDUAL_PAGE_NUMS_WIDTH 30

using namespace std;
class VideoBrowser{
    const string KEYWORDS_LIST_START = "Keywords: ";
    const string KEYWORDS_INPUT_PROMPT = "Add keywords";
    
    const string COLOR_DISPLAY = "Color (hue): ";
    const string LUMINANCE_DISPLAY = "Luminance: ";
    const string NUM_FACES_DISPLAY = "Num. faces (avg): ";
    const string RYTHM_DISPLAY = "Rythm: ";
    
    const string EDGE_DIST_DISPLAY = "Edge distribution: ";
    const string EDGE_DIST_VERT = "Vertical: ";
    const string EDGE_DIST_HOR = "Horizontal: ";
    const string EDGE_DIST_DIA45 = "Diagonal 45deg: ";
    const string EDGE_DIST_DIA135 = "Diagonal 135deg: ";
    
    
    string selectedVideoName;
    string metadataFile;
    
    string keywordsList;
    string videoColor;
    string videoLuminance;
    string videoNumFaces;
    string videoRythm;
    string videoEdgesVert, videoEdgesHor, videoEdgesDia45, videoEdgesDia135;
    
    float xPos, yPos;
    float metadataXPos, metadataYPos;
    float width, height;
    float videoW, videoH;

    int maxElementsPerRow;
    int maxRows;
    int currentPage;
    
    bool displayMetadata;
    bool videoSelected;
    
    vector<vector<shared_ptr<Thumbnail> > > thumbnailsByPage;
    
    // For easy access when updating preview frames
    unordered_map<string, shared_ptr<Thumbnail> > thumbnailsByPath;
    
    vector<unique_ptr<TextToggle> > pageNumbers;
    
    ofxInputField<string> addKeywordInput;
    
    ofxXmlSettings *XML;
    
public:
    VideoBrowser(float x, float y, float w, float h, float elementW, float elementH, vector<string> videoPaths, shared_ptr<Player> player);
    
    void draw();
    
    void onPageNumClick(string & txt);
    
    void updatePreviewFrames(string vidPath, vector<ofTexture> frames);
    
    void update();
    
    void setThumbnailEnabled(string path, bool isEnabled);
    void setThumbnailVisible(string path, bool isVisible);
    
    void setupMetadataDisplay(float x, float y, ofxXmlSettings &xml);
    
protected:
    void drawMetadataDisplay();
    void onThumbnailSelected(string & videoName);
    void onKeywordsInputSubmit(string & input);
};
