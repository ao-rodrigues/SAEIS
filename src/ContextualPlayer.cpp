#include "ContextualPlayer.h"

ContextualPlayer::ContextualPlayer(string behavioursXml) {
    if(behavioursXML.loadFile(behavioursXml)) {
        cout << "Behaviours XML loaded!\n";
    } else {
        cout << "Behaviours XML not loaded!\n";
    }
    
    camera.setDeviceID(0);
    camera.setDesiredFrameRate(30);
    camera.initGrabber(CAMERA_WIDTH, CAMERA_HEIGHT);
    
    player.setLoopState(OF_LOOP_NORMAL);
    isFullscreen = false;
    
    videoAreaXPos = ofGetWindowWidth() - PLAYER_WIDTH - 50;
    videoAreaYPos = 50;
    
    videoAreaWidth = PLAYER_WIDTH;
    videoAreaHeight = PLAYER_HEIGHT;
    
    videoStart = ofGetElapsedTimeMillis();
    
    
    ofSetVerticalSync(true);
}

void ContextualPlayer::update() {
    camera.update();
    
    if(camera.isFrameNew()) {
        processFrame(camera.getPixels());
    }
}

void ContextualPlayer::draw() {
    if(!player.isLoaded()) {
        ofSetColor(ofColor::gray);
        ofDrawRectangle(videoAreaXPos, videoAreaYPos, videoAreaWidth, videoAreaHeight);
    } else {
        ofSetColor(ofColor::black);
        ofDrawRectangle(videoAreaXPos, videoAreaYPos, videoAreaWidth, videoAreaHeight);
        
        ofSetColor(ofColor::white);
        player.draw(videoXPos, videoYPos, videoWidth, videoHeight);
    }
    
    
    if(!isFullscreen) {
        ofSetColor(ofColor::white);
        camera.draw(ofGetWindowWidth() - camera.getWidth() - 50, ofGetWindowHeight() - camera.getHeight() - 50);
    }
    
}

void ContextualPlayer::setFullscreen(bool setFullscreen) {
    isFullscreen = setFullscreen;
    
    if(isFullscreen) {
        videoAreaXPos = 0;
        videoAreaYPos = 0;
        
        videoAreaWidth = ofGetWindowWidth();
        videoAreaHeight = ofGetWindowHeight();
    } else {
        videoAreaXPos = ofGetWindowWidth() - PLAYER_WIDTH - 50;
        videoAreaYPos = 50;
        
        videoAreaWidth = PLAYER_WIDTH;
        videoAreaHeight = PLAYER_HEIGHT;
    }
    
    calculatePlayerDimensions();
}

void ContextualPlayer::loadVideo(const string path) {
    if (player.isLoaded()) {
        player.closeMovie();
    }
    
    player.load(path);
    calculatePlayerDimensions();
    
    player.play();
    videoStart = ofGetElapsedTimeMillis();

}

void ContextualPlayer::calculatePlayerDimensions() {
    if(player.isLoaded()) {
        float vidW = player.getWidth();
        float vidH = player.getHeight();
        
        float aspectRatio = vidW / vidH;
        
        if(vidW > vidH) {
            // Letterboxing
            videoWidth = videoAreaWidth;
            videoHeight = videoAreaWidth / aspectRatio;
            
            videoXPos = videoAreaXPos;
            videoYPos = videoAreaYPos + ((videoAreaHeight - videoHeight) / 2.0);
        } else {
            // Pillarboxing
            videoWidth = videoAreaWidth * aspectRatio;
            videoHeight = videoAreaHeight;
            
            videoXPos = videoAreaXPos + ((videoAreaWidth - videoWidth) / 2.0);
            videoYPos = videoAreaYPos;
        }
    }
}

void ContextualPlayer::processFrame(ofPixels &pixels) {
    uint64_t currTime = ofGetElapsedTimeMillis();
    
    if(currTime - videoStart > VIDEO_PLAY_TIME_MILLIS) {
        // The video has played for 5 seconds, we can process the camera again
        vector<string> events;
        
        
    }
}
