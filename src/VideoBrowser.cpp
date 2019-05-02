#include "VideoBrowser.h"
#include <cmath>

using namespace std;

// This implementation assumes that there's space for at least one element per page
VideoBrowser::VideoBrowser(float x, float y, float w, float h, float elementW, float elementH, vector<string> videoPaths)
: xPos(x), yPos(y), width(w), height(h), videoW(elementW - 2 * SPACING_X), videoH(elementH - 2 * SPACING_Y), currentPage(1) {
    
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
    
    for (int i = 0; i < numPages; i++) {
        vector<Thumbnail> v;
        thumbnails.push_back(v);
    }
    
    for (string p : videoPaths) {
        ofVideoPlayer tmp;
        tmp.load(p);
        //tmp.play();
        //tmp.update();
        tmp.update();
        tmp.setFrame(0);
        
        ofImage img;
        unsigned char * pixels = tmp.getPixels().getData();
        img.setFromPixels(pixels, tmp.getWidth(), tmp.getHeight(), OF_IMAGE_COLOR);
        
        Thumbnail th (thumbX, thumbY, videoW, videoH, img, p, ofColor::black);
        
        // TODO
        //ofAddListener(t.clickedInside, &AppController, &AppController::handleThumbnailClick);
        
        thumbnails[currPageIdx].push_back(th);

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
                TextToggle tt (xPos + currPageIdx * INDIVIDUAL_PAGE_NUMS_WIDTH, yPos + thumbY, INDIVIDUAL_PAGE_NUMS_WIDTH, PAGE_NUMS_HEIGHT, to_string(currPageIdx + 1), false);
                ofAddListener(tt.textClicked, this, &VideoBrowser::onPageNumClick);
                
                pageNumbers.push_back(tt);
                
                currPageIdx++;
                
                thumbY = yPos;
                nextThumbY = thumbY + elementH;
            }
        }
    }
    
    // Starts displaying first page
    pageNumbers[0].toggle();
    
}

void VideoBrowser::draw(){
    for (Thumbnail t : thumbnails[currentPage - 1]){
        t.draw();
    }
    
    for (TextToggle t : pageNumbers) {
        t.draw();
    }
}

void VideoBrowser::onPageNumClick(string & num) {
    currentPage = std::stoi(num);
}

