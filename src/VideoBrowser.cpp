#include "VideoBrowser.h"
#include <cmath>

using namespace std;

// This implementation assumes that there's space for at least one element per page
VideoBrowser::VideoBrowser(float x, float y, float w, float h, float elementW, float elementH, vector<string> videoPaths, shared_ptr<Player> player)
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
    thumbnailsByPage.reserve(numPages);
    pageNumbers.reserve(numPages);
    
    for (int i = 0; i < numPages; i++) {
        vector<shared_ptr<Thumbnail> > v;
        v.reserve(maxElementsPerRow * maxRows);
        
        thumbnailsByPage.push_back(move(v));
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
        shared_ptr<Thumbnail> thPtr = make_shared<Thumbnail>(thumbX, thumbY, videoW, videoH, p, player);

        thumbnailsByPage[currPageIdx].push_back(thPtr);
        thumbnailsByPath.insert(make_pair(p, thPtr));
        
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
    
    for(int i = 0; i < thumbnailsByPage[currentPage - 1].size(); i++) {
        thumbnailsByPage[currentPage - 1][i]->setEnabled(true);
    }
        
}

void VideoBrowser::update() {
    for(int i = 0; i < thumbnailsByPage.size(); i++) {
        thumbnailsByPage[currentPage - 1][i]->update();
    }
}

void VideoBrowser::draw(){
    for (int i = 0; i < thumbnailsByPage[currentPage - 1].size(); i++){
        thumbnailsByPage[currentPage - 1][i]->draw();
    }
    
    for (int i = 0; i < pageNumbers.size(); i++) {
        pageNumbers[i]->draw();
    }
}


void VideoBrowser::onPageNumClick(string & txt) {
    //Toggle currently selected off
    pageNumbers[currentPage - 1]->setToggled(false);
    
    for(int i = 0; i < thumbnailsByPage[currentPage - 1].size(); i++) {
        thumbnailsByPage[currentPage - 1][i]->setEnabled(false);
    }
    
    for(int i = 0; i < pageNumbers.size(); i++) {
        if (pageNumbers[i]->getText().compare(txt) == 0) {
            
            // Toggle new on
            pageNumbers[i]->setToggled(true);
            currentPage = stoi(txt);
        }
    }
    
    for(int i = 0; i < thumbnailsByPage[currentPage - 1].size(); i++) {
        thumbnailsByPage[currentPage - 1][i]->setEnabled(true);
    }
        
}

void VideoBrowser::updatePreviewFrames(string vidPath, vector<ofTexture> frames) {
    unordered_map<string, shared_ptr<Thumbnail> >::const_iterator found = thumbnailsByPath.find(vidPath);
    
    found->second->updatePreviewFrames(frames);
}

void VideoBrowser::setThumbnailEnabled(string path, bool isEnabled) {
    unordered_map<string, shared_ptr<Thumbnail> >::const_iterator found = thumbnailsByPath.find(path);
    found->second->setEnabled(isEnabled);
}

