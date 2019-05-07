#include "VideoBrowser.h"
#include <cmath>

using namespace std;

// This implementation assumes that there's space for at least one element per page
VideoBrowser::VideoBrowser(float x, float y, float w, float h, float elementW, float elementH, vector<string> videoPaths, shared_ptr<ofVideoPlayer> player)
: xPos(x), yPos(y), width(w), height(h), videoW(elementW - 2 * SPACING_X), videoH(elementH - 2 * SPACING_Y) {
    
    if(videoW > width || videoH > height) {
        // Random exception number
        throw 20;
    }
    
    // Save some space for the pagination numbers
    float videoAreaHeight = height - PAGE_NUMS_HEIGHT;
    
    maxElementsPerRow = (int)floor(width / elementW);
    maxRows = (int)floor(videoAreaHeight / elementH);
    
    int numPages = ceil((float)videoPaths.size() / (float)(maxElementsPerRow * maxRows));
    thumbnails.reserve(numPages);
    pageNumbers.reserve(numPages);
    
    for (int i = 0; i < numPages; i++) {
        vector<unique_ptr<Thumbnail> > v;
        v.reserve(maxElementsPerRow * maxRows);
        
        thumbnails.push_back(move(v));
    }
    
    int elementsInRow = 0;
    int numRows = 0;
    
    float thumbX = xPos;
    float thumbY = yPos;
    
    // Used to check ahead if there's space for one more
    float nextThumbX = thumbX + elementW;
    float nextThumbY = thumbY + elementH;
    
    int currPageIdx = 0;
    
    // Add first page
    pageNumbers.push_back(make_unique<TextToggle>(xPos + (currPageIdx * INDIVIDUAL_PAGE_NUMS_WIDTH),
                                                  yPos + videoAreaHeight,
                                                  INDIVIDUAL_PAGE_NUMS_WIDTH,
                                                  PAGE_NUMS_HEIGHT,
                                                  to_string(currPageIdx + 1),
                                                  false));
    ofAddListener(pageNumbers.back()->textClicked, this, &VideoBrowser::onPageNumClick);

    //currPageIdx++;
    
    for (string p : videoPaths) {
        unique_ptr<Thumbnail> thPtr = make_unique<Thumbnail>(thumbX, thumbY, videoW, videoH, p, player);
        thumbnails[currPageIdx].push_back(move(thPtr));
        //ofAddListener(thumbnails[currPageIdx].back()->videoClicked, this, &VideoBrowser::onVideoClicked);
        
        elementsInRow++;
        
        // Check if we reached the end of current row
        if(elementsInRow >= maxElementsPerRow) {
            thumbX = xPos;
            nextThumbX = thumbX + elementW;
            
            numRows++;
            elementsInRow = 0;
            
            // Check if we reached row limit and need to add a new page
            if(numRows >= maxRows) {
                currPageIdx++;
                unique_ptr<TextToggle> ttPtr = make_unique<TextToggle>(xPos + (currPageIdx * INDIVIDUAL_PAGE_NUMS_WIDTH),
                                                                       yPos + videoAreaHeight,
                                                                       INDIVIDUAL_PAGE_NUMS_WIDTH,
                                                                       PAGE_NUMS_HEIGHT,
                                                                       to_string(currPageIdx + 1),
                                                                       false);
                
                pageNumbers.push_back(move(ttPtr));
                ofAddListener(pageNumbers.back()->textClicked, this, &VideoBrowser::onPageNumClick);
                
                thumbY = yPos;
                nextThumbY = thumbY + elementH;
                
                elementsInRow = 0;
                numRows = 0;
            } else {
                thumbY = nextThumbY;
                nextThumbY += elementH;
            }
        } else {
            thumbX = nextThumbX;
            nextThumbX += elementW;
        }
    }
    
    // Starts displaying first page
    pageNumbers[0]->setToggled(true);
    currentPage = 1;
    
    for(int i = 0; i < thumbnails[currentPage - 1].size(); i++) {
        thumbnails[currentPage - 1][i]->setEnabled(true);
    }
        
}

void VideoBrowser::update() {
    for(int i = 0; i < thumbnails.size(); i++) {
        thumbnails[currentPage - 1][i]->update();
    }
}

void VideoBrowser::draw(){
    /*
    ofSetColor(ofColor::red);
    ofDrawRectangle(xPos - 3, yPos - 3, width + 6, height + 6);
    
    ofSetColor(ofColor::white);
    ofDrawRectangle(xPos, yPos, width, height);
    */
    for (int i = 0; i < thumbnails[currentPage - 1].size(); i++){
        thumbnails[currentPage - 1][i]->draw();
    }
    
    for (int i = 0; i < pageNumbers.size(); i++) {
        pageNumbers[i]->draw();
    }
}


void VideoBrowser::onPageNumClick(string & txt) {
    //Toggle currently selected off
    pageNumbers[currentPage - 1]->setToggled(false);
    
    for(int i = 0; i < thumbnails[currentPage - 1].size(); i++) {
        thumbnails[currentPage - 1][i]->setEnabled(false);
    }
    
    for(int i = 0; i < pageNumbers.size(); i++) {
        if (pageNumbers[i]->getText().compare(txt) == 0) {
            
            // Toggle new on
            pageNumbers[i]->setToggled(true);
            currentPage = stoi(txt);
        }
    }
    
    for(int i = 0; i < thumbnails[currentPage - 1].size(); i++) {
        thumbnails[currentPage - 1][i]->setEnabled(true);
    }
        
}

/*
void VideoBrowser::onVideoClicked(const int & thumbnailId) {
    // This is horrible but it's the best way for now
    for(int i = 0; i < thumbnails.size(); i++) {
        for (int j = 0; j < thumbnails[i].size(); j++) {
            if(thumbnailId != thumbnails[i][j]->getId()) {
                thumbnails[i][j]->setSelected(false);
            } else {
                thumbnails[i][j]->setSelected(true);
            }
        }
    }
}
 */

