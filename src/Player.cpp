#include "Player.h"

Player::Player()
: xPos(0), yPos(0), videoXPos(0), videoYPos(0), width(1024), height(768), videoWidth(1024), videoHeight(1024){
    vidPath = "";
}

Player::Player(float x, float y, float w, float h)
: xPos(x), yPos(y), width(w), height(h){
    vidPath = "";
    player.setLoopState(OF_LOOP_NORMAL);
}

void Player::load(const string path) {
    if (player.isLoaded()) {
        player.closeMovie();
    }
    
    player.load(path);
    vidPath = path;
    
    float vidW = player.getWidth();
    float vidH = player.getHeight();
    
    float aspectRatio = vidW / vidH;
    
    if (vidW > vidH) {
        // Letterboxing
        videoWidth = width;
        videoHeight = width / aspectRatio;
        
        videoXPos = xPos;
        videoYPos = yPos + ((height - videoHeight) / 2.0);
    } else {
        // Pillarboxing
        videoWidth = height * aspectRatio;
        videoHeight = height;
        
        videoXPos = xPos + ((width - videoWidth) / 2.0);
        videoYPos = yPos;
    }

}

void Player::play() {
    player.play();
}

void Player::keyPress(int key) {
    float fps = player.getSpeed();
    switch (key) {
            // play / pause
        case OF_KEY_F1:
            player.setPaused(player.isPlaying());
            break;
            
        default:
            break;
    }
}

void Player::draw() {
    ofSetColor(ofColor::black);
    ofDrawBitmapString("Now Playing: " + vidPath, xPos, yPos - 15);
    
    if (player.isLoaded()){
        ofSetColor(ofColor::black);
        ofDrawRectangle(xPos, yPos, width, height);
        
        ofSetColor(ofColor::white);
        player.draw(videoXPos,
                    videoYPos,
                    videoWidth,
                    videoHeight);
    } else {
        ofSetColor(ofColor::gray);
        ofDrawRectangle(xPos, yPos, width, height);
    }
}

void Player::update() {
    if(player.isLoaded()) {
        player.update();
    }
    
}
