#include "TextToggle.h"

TextToggle::TextToggle(float x, float y, float w, float h, string txt)
: xPos(x), yPos(y), width(w), height(h), text(txt), toggled(false) {
    ofRegisterMouseEvents(this);
}

void TextToggle::draw() {
    ofSetColor(ofColor::black);
    if(toggled) {
        ofDrawBitmapString("[" + text + "]", xPos, yPos);
    } else {
        ofDrawBitmapString(text, xPos, yPos);
    }
}

void TextToggle::mouseMoved(ofMouseEventArgs &args){}
void TextToggle::mouseDragged(ofMouseEventArgs &args){}
void TextToggle::mousePressed(ofMouseEventArgs & args){}
void TextToggle::mouseReleased(ofMouseEventArgs & args) {
    if(inside(args.x, args.y)) {
        toggled = !toggled;
        
        ofNotifyEvent(textClicked, text, this);
    }
}

void TextToggle::mouseScrolled(ofMouseEventArgs & args){}
void TextToggle::mouseEntered(ofMouseEventArgs & args){}
void TextToggle::mouseExited(ofMouseEventArgs & args){}

bool TextToggle::inside(float x, float y) {
    return (xPos <= x && x <= xPos + width) && (yPos <= y && y <= height);
}