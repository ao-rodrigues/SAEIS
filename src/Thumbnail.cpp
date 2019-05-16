#include "Thumbnail.h"

const string Thumbnail::THUMBNAILS_PATH = "thumbnails/thumbnail_";

Thumbnail::Thumbnail(float x, float y, float w, float h, string vidPath, shared_ptr<Player> plyr)
: xPos(x), yPos(y), width(w), height(h - NAME_HEIGHT), videoPath(vidPath), selected(false), player(plyr){
    ofRegisterMouseEvents(this);
    
    //videoPlayer.load(videoName);
    //videoPlayer.setLoopState(OF_LOOP_NORMAL);
    hasPreviewFrames = false;
    
    videoName = extractVideoName(videoPath);
    enabled = false;
    
    /*
    bool success = thumbnail.load(THUMBNAILS_PATH + videoName + ".png");
    
    float imgW, imgH;
    
    if(success) {
        imgW = thumbnail.getWidth();
        imgH = thumbnail.getHeight();
    
        
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
        
    } else {
        fixedW = width;
        fixedH = height;
        fixedXPos = xPos;
        fixedYPos = yPos;
        
        // Make a gray placeholder image
        thumbnail.allocate(fixedW, fixedH, OF_IMAGE_COLOR);
        thumbnail.setColor(ofColor::gray);
    }
     */
    
    clickListener = ofEvents().mouseReleased.newListener(this, &Thumbnail::onMouseReleased);
    
    
}

void Thumbnail::update() {
    //videoPlayer.update();
}

bool Thumbnail::mouseOver() {
    return inside(ofGetMouseX(), ofGetMouseY());
}

void Thumbnail::draw() {
    if(selected) {
        // Draw red border around Thumbnail
        ofSetColor(ofColor::red);
        ofDrawRectangle(xPos - BORDER_WIDTH, yPos - BORDER_WIDTH, width + 2 * BORDER_WIDTH, height + 2 * BORDER_WIDTH);
    }
    
    if(hasPreviewFrames){
        // Black background for possible letterboxing/pillarboxing
        ofSetColor(ofColor::black);
        ofDrawRectangle(xPos, yPos, width, height);
        
        ofTexture frame;
        
        if (mouseOver()){
            // Start frames sequence
            
            // Get number of seconds from application start
            float time = ofGetElapsedTimef();
            
            size_t n = previewFrames.size();
            
            // Get sequence duration
            float duration = n / PREVIEW_FPS;
            
            float pos = fmodf(time, duration);
            
            // Calculate index of frame to show
            int i = int(pos / duration * n);
            
            frame = previewFrames[i];
        } else {
            // Show first frame
            frame = previewFrames[0];
        }
        
        float fixedXPos, fixedYPos, fixedW, fixedH;
        calcFixedDimensions(frame, fixedXPos, fixedYPos, fixedW, fixedH);
        
        ofSetColor(ofColor::white);
        frame.draw(fixedXPos, fixedYPos, fixedW, fixedH);
    } else {
        // Draw gray rectangle as placeholder
        ofSetColor(ofColor::gray);
        ofDrawRectangle(xPos, yPos, width, height);
    }
    
    ofSetColor(ofColor::black);
    ofDrawBitmapString(videoName, xPos, yPos + height + NAME_HEIGHT);
}

void Thumbnail::updatePreviewFrames(vector<ofTexture> frames) {
    previewFrames = frames;
    hasPreviewFrames = true;
}


void Thumbnail::mouseMoved(ofMouseEventArgs & args){}
void Thumbnail::mouseDragged(ofMouseEventArgs & args){}
void Thumbnail::mousePressed(ofMouseEventArgs & args){}
void Thumbnail::mouseReleased(ofMouseEventArgs & args){}
void Thumbnail::mouseScrolled(ofMouseEventArgs &args){}
void Thumbnail::mouseEntered(ofMouseEventArgs &args){}
void Thumbnail::mouseExited(ofMouseEventArgs &args){}

void Thumbnail::onMouseReleased(ofMouseEventArgs &args) {
    if(inside(args.x, args.y) && enabled) {
        // Toggle selected status
        selected = true;
        
        player->load(videoPath);
        player->play();
        
        //ofNotifyEvent(videoClicked, thumbnailId, this);
    } else {
        selected = false;
    }
}
    

bool Thumbnail::inside(float x, float y){
    return (xPos <= x && x <= xPos + width) && (yPos <= y && y <= yPos + height);
}

void Thumbnail::setSelected(bool isSelected) {
    selected = isSelected;
}

void Thumbnail::setEnabled(bool isEnabled) {
    enabled = isEnabled;
}


string Thumbnail::extractVideoName(string path) {
    string baseFilename = path.substr(path.find_last_of("/\\") + 1);
    std::size_t extStart = baseFilename.find_last_of(".");
    
    return baseFilename.substr(0, extStart);
}

void Thumbnail::calcFixedDimensions(ofTexture frame, float & fixedXPos, float & fixedYPos, float & fixedW, float & fixedH) {
    float imgW, imgH;
    
    imgW = frame.getWidth();
    imgH = frame.getHeight();
    
    
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

