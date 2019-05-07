#include "TextToggle.h"

TextToggle::TextToggle(float x, float y, float w, float h, string txt, bool startToggled)
: xPos(x), yPos(y), width(w), height(h), text(txt), toggled(startToggled) {
    ofRegisterMouseEvents(this);
}

void TextToggle::draw() {
    /*
    ofSetColor(ofColor::red);
    ofDrawRectangle(xPos - 2, yPos - 2, width + 4, height + 4);
    
    ofSetColor(ofColor::white);
    ofDrawRectangle(xPos, yPos, width, height);
    */
    
    ofSetColor(ofColor::black);
    if(toggled) {
        ofDrawBitmapString("[" + text + "]", xPos, yPos + height);
    } else {
        ofDrawBitmapString(text, xPos, yPos + height);
    }
}

void TextToggle::mouseMoved(ofMouseEventArgs &args){}
void TextToggle::mouseDragged(ofMouseEventArgs &args){}
void TextToggle::mousePressed(ofMouseEventArgs & args){}
void TextToggle::mouseReleased(ofMouseEventArgs & args) {
    if(inside(args.x, args.y) && !toggled) {
        //toggle();
        ofNotifyEvent(textClicked, text, this);
    }
}

void TextToggle::mouseScrolled(ofMouseEventArgs & args){}
void TextToggle::mouseEntered(ofMouseEventArgs & args){}
void TextToggle::mouseExited(ofMouseEventArgs & args){}

bool TextToggle::inside(float x, float y) {
    return (xPos <= x && x <= xPos + width) && (yPos <= y && y <= yPos + height);
}

void TextToggle::setToggled(bool isToggled) {
    toggled = isToggled;
}

string TextToggle::getText() {
    return text;
}
