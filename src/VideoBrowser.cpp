#include "VideoBrowser.h"
#include <cmath>

using namespace std;

// This implementation assumes that there's space for at least one element per page
VideoBrowser::VideoBrowser(float x, float y, float w, float h, float elementW, float elementH, vector<string> videoPaths)
: xPos(x), yPos(y), width(w), height(h), videoW(elementW - 2 * SPACING_X), videoH(elementH - 2 * SPACING_Y) {
    
    if(videoW > width || videoH > height) {
        // Random exception number
        throw 20;
    }
    
    // Save some space for the pagination numbers
    float videoAreaHeight = height - PAGE_NUMS_HEIGHT;
    
    maxElementsPerRow = (int)floor(width / elementW);
    maxElementsPerCol = (int)floor(height / elementH);
    
    float thumbX = xPos;
    float thumbY = yPos;
    
    // Used to check ahead if there's space for one more
    float nextThumbX = thumbX + elementW;
    float nextThumbY = thumbY + elementH;

    int currPageIdx = 0;
    
    int numPages = ceil((float)videoPaths.size() / (float)(maxElementsPerRow * maxElementsPerCol));
    thumbnails.reserve(numPages);
    
    for (int i = 0; i < numPages; i++) {
        vector<unique_ptr<Thumbnail> > v;
        v.reserve(maxElementsPerRow * maxElementsPerCol);
        
        thumbnails.push_back(move(v));
    }
    
    for (string p : videoPaths) {
        unique_ptr<Thumbnail> thPtr = make_unique<Thumbnail>(thumbX, thumbY, videoW, videoH, p, ofColor::black);
        thumbnails[currPageIdx].push_back(move(thPtr));

        thumbX = nextThumbX;
        nextThumbX += elementW;
        
        // Check if we reached the end of current row
        if(nextThumbX > width) {
            thumbX = xPos;
            nextThumbX = thumbX + elementW;
            
            thumbY = nextThumbY;
            nextThumbY += elementH;
            
            // Check if we reached row limit and need to add a new page
            if(nextThumbY > videoAreaHeight) {
                unique_ptr<TextToggle> ttPtr = make_unique<TextToggle>(xPos + currPageIdx * INDIVIDUAL_PAGE_NUMS_WIDTH,
                                                                       yPos + thumbY,
                                                                       INDIVIDUAL_PAGE_NUMS_WIDTH,
                                                                       PAGE_NUMS_HEIGHT,
                                                                       to_string(currPageIdx + 1),
                                                                       false);
                
                pageNumbers.push_back(move(ttPtr));
                ofAddListener(pageNumbers.back()->textClicked, this, &VideoBrowser::onPageNumClick);
                
                currPageIdx++;
                
                thumbY = yPos;
                nextThumbY = thumbY + elementH;
            }
        }
    }
    
    // Starts displaying first page
    pageNumbers[0]->toggle();
    currentPage = 1;
}

void VideoBrowser::update() {
    for(int i = 0; i < thumbnails.size(); i++) {
        thumbnails[currentPage - 1][i]->update();
    }
}

void VideoBrowser::draw(){
    for (int i = 0; i < thumbnails[currentPage - 1].size(); i++){
        thumbnails[currentPage - 1][i]->draw();
    }
    
    for (int i = 0; i < pageNumbers.size(); i++) {
        pageNumbers[i]->draw();
    }
}

void VideoBrowser::onPageNumClick(string & num) {
    cout << "Page num clicked!";
    currentPage = stoi(num);
}

