#include "Thumbnail.h"

Thumbnail::Thumbnail(float w, float h, ofImage img, string name, ofColor txtColor)
: width(w), height(h + NAME_HEIGHT), image(img), videoName(name), textColor(txtColor)
{}

void Thumbnail::draw(float x, float y) {
    // Black background for possible letterboxing
    ofSetColor(ofColor::black);
    ofDrawRectangle(x, y, width, height - NAME_HEIGHT);
    
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
    image.draw(x, y, fixedW, fixedH);
    
    ofSetColor(textColor);
    ofDrawBitmapString(videoName, x, y + height + NAME_HEIGHT);
}
