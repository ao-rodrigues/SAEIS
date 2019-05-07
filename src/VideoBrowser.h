#pragma once

#include "ofMain.h"
#include "Thumbnail.h"
#include "TextToggle.h"
#include "Player.h"

#define SPACING_X 10
#define SPACING_Y 10
#define PAGE_NUMS_HEIGHT 15
#define INDIVIDUAL_PAGE_NUMS_WIDTH 30

using namespace std;
class VideoBrowser{
    float xPos, yPos;
    float width, height;
    float videoW, videoH;

    int maxElementsPerRow;
    int maxRows;
    int currentPage;
    
    vector<vector<unique_ptr<Thumbnail> > > thumbnails;
    vector<unique_ptr<TextToggle> > pageNumbers;
    
    
    
    //unique_ptr<ofVideoPlayer> nowPlaying;
    
    //ofVec2f clickedPoint;
    
public:
    VideoBrowser(float x, float y, float w, float h, float elementW, float elementH, vector<string> videoPaths, shared_ptr<Player> player);
    void draw();
    void onPageNumClick(string & txt);
    void onVideoClicked(const int & thumbnailId);
    
    void update();
};
