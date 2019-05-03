#include "Thumbnail.h"

Thumbnail::Thumbnail(float x, float y, float w, float h, string vidPath, ofColor txtColor)
: xPos(x), yPos(y), width(w), height(h - NAME_HEIGHT), videoPath(vidPath), textColor(txtColor), selected(false){
    ofRegisterMouseEvents(this);
    
    videoPlayer.load(videoPath);
    videoPlayer.setLoopState(OF_LOOP_NORMAL);
    
    float imgW = videoPlayer.getWidth();
    float imgH = videoPlayer.getHeight();
    
    float aspectRatio = imgW / imgH;
    
    if(imgW > imgH) {
        // Letterboxing
        fixedW = width;
        fixedH = width / aspectRatio;
        
        fixedXPos = xPos;
        fixedYPos = yPos + ((height - fixedH) / 2.0);
        
    } else {
        // Pillarboxing
        fixedW = height * aspectRatio;
        fixedH = height;
        
        fixedXPos = xPos + ((width - fixedW) / 2.0);
        fixedYPos = yPos;
    }
}

void Thumbnail::update() {
    videoPlayer.update();
}

void Thumbnail::draw() {
    if(selected) {
        // Draw red border around Thumbnail
        ofSetColor(ofColor::red);
        ofDrawRectangle(xPos - BORDER_WIDTH, yPos - BORDER_WIDTH, width + BORDER_WIDTH, height + BORDER_WIDTH);
    }
    
    // Black background for possible letterboxing/pillarboxing
    ofSetColor(ofColor::black);
    ofDrawRectangle(xPos, yPos, width, height);
    
    ofSetColor(ofColor::white);
    videoPlayer.draw(fixedXPos, fixedYPos, fixedW, fixedH);
    //ofDrawRectangle(fixedXPos, fixedYPos, fixedW, fixedH);
    
    ofSetColor(textColor);
    ofDrawBitmapString(videoPath, xPos, yPos + height + NAME_HEIGHT);
}

void Thumbnail::mouseMoved(ofMouseEventArgs & args){}
void Thumbnail::mouseDragged(ofMouseEventArgs & args){}
void Thumbnail::mousePressed(ofMouseEventArgs & args){}
void Thumbnail::mouseReleased(ofMouseEventArgs & args){
    if(inside(args.x, args.y)) {
        // Toggle selected status
        selected = !selected;
        
        if(selected) {
            videoPlayer.play();
        }
        
        //ofVec2f mousePos = ofVec2f(args.x, args.y);
        
        //ofVideoPlayer * playerPtr = &videoPlayer;
        //ofNotifyEvent(videoClicked, videoPlayer, this);
    }
    
}

void Thumbnail::mouseScrolled(ofMouseEventArgs &args){}
void Thumbnail::mouseEntered(ofMouseEventArgs &args){}
void Thumbnail::mouseExited(ofMouseEventArgs &args){}

bool Thumbnail::inside(float x, float y){
    return (xPos <= x && x <= xPos + width) && (yPos <= y && y <= yPos + height);
}
