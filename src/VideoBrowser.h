#pragma once

#include "ofMain.h"
#include "Thumbnail.h"
#include "TextToggle.h"

#define SPACING_X 10
#define SPACING_Y 10
#define PAGE_NUMS_HEIGHT 10
#define INDIVIDUAL_PAGE_NUMS_WIDTH 5

using namespace std;
class VideoBrowser{
    float xPos, yPos;
    float width, height;
    float videoW, videoH;

    
    int maxElementsPerRow;
    int maxElementsPerCol;
    
    vector<vector<Thumbnail> > thumbnails;
    vector<TextToggle> pageNumbers;
    
    size_t currentPage;
    
    //unique_ptr<ofVideoPlayer> nowPlaying;
    
    //ofVec2f clickedPoint;
    
public:
    VideoBrowser(float x, float y, float w, float h, float elementW, float elementH, vector<string> videoPaths);
    void draw();
    void onPageNumClick(string & txt);
    void onVideoClicked(ofVideoPlayer & player);
    
    void update();
};
