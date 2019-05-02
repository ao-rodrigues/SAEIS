#include "Thumbnail.h"

Thumbnail::Thumbnail(float x, float y, float w, float h, ofImage img, string name, ofColor txtColor)
: xPos(x), yPos(y), width(w), height(h), image(img), videoName(name), textColor(txtColor), selected(false){
    ofRegisterMouseEvents(this);
}

void Thumbnail::draw() {
    if(selected) {
        // Draw red border around Thumbnail
        ofSetColor(ofColor::red);
        ofDrawRectangle(xPos - BORDER_WIDTH, yPos - BORDER_WIDTH, width + BORDER_WIDTH, height + BORDER_WIDTH);
    }
    
    // Black background for possible letterboxing
    ofSetColor(ofColor::black);
    ofDrawRectangle(xPos, yPos, width, height - NAME_HEIGHT);
    
    float imgW = image.getWidth();
    float imgH = image.getHeight();
    
    float aspectRatio = imgW / imgH;
    
    float fixedW, fixedH;
    
    if(imgW > width) {
        fixedW = width;
        fixedH = width / aspectRatio;
    } else {
        fixedW = height * aspectRatio;
        fixedH = height;
    }
    
    ofSetColor(ofColor::white);
    image.draw(xPos, yPos, fixedW, fixedH);
    
    ofSetColor(textColor);
    ofDrawBitmapString(videoName, xPos, yPos + height);
}

void Thumbnail::mouseMoved(ofMouseEventArgs & args){}
void Thumbnail::mouseDragged(ofMouseEventArgs & args){}
void Thumbnail::mousePressed(ofMouseEventArgs & args){}
void Thumbnail::mouseReleased(ofMouseEventArgs & args){
    if(inside(args.x, args.y)) {
        // Toggle selected status
        selected = !selected;
        
        ofVec2f mousePos = ofVec2f(args.x, args.y);
        ofNotifyEvent(clickedInside, mousePos, this);
    }
    
}

void Thumbnail::mouseScrolled(ofMouseEventArgs &args){}
void Thumbnail::mouseEntered(ofMouseEventArgs &args){}
void Thumbnail::mouseExited(ofMouseEventArgs &args){}

bool Thumbnail::inside(float x, float y){
    return (xPos <= x && x <= xPos + width) && (yPos <= y && y <= yPos + height);
}
