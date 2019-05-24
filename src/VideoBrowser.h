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
    
    string keywordsList;
    string selectedVideoName;
    string metadataFile;
    
    float xPos, yPos;
    float tagsXPos, tagsYPos;
    float width, height;
    float videoW, videoH;

    int maxElementsPerRow;
    int maxRows;
    int currentPage;
    
    bool displayKeywords;
    bool videoSelected;
    
    vector<vector<shared_ptr<Thumbnail> > > thumbnailsByPage;
    
    // For easy access when updating preview frames
    unordered_map<string, shared_ptr<Thumbnail> > thumbnailsByPath;
    
    vector<unique_ptr<TextToggle> > pageNumbers;
    
    ofxInputField<string> addKeywordInput;
    
    ofxXmlSettings XML;
    
public:
    VideoBrowser(float x, float y, float w, float h, float elementW, float elementH, vector<string> videoPaths, shared_ptr<Player> player);
    
    void draw();
    
    void onPageNumClick(string & txt);
    
    void updatePreviewFrames(string vidPath, vector<ofTexture> frames);
    
    void update();
    
    void setThumbnailEnabled(string path, bool isEnabled);
    
    void setupKeywordsDisplay(float x, float y, string metadataFile);
    
protected:
    void drawKeywordsDisplay();
    void onThumbnailSelected(string & videoName);
    void onKeywordsInputSubmit(string & input);
};
