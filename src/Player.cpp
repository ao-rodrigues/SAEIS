#include "Player.h"

Player::Player()
: xPos(0), yPos(0), videoXPos(0), videoYPos(0), width(1024), height(768), videoWidth(1024), videoHeight(1024){}

Player::Player(float x, float y, float w, float h)
: xPos(x), yPos(y), width(w), height(h){
    player.setLoopState(OF_LOOP_NORMAL);
}

void Player::load(const string path) {
    if (player.isLoaded()) {
        player.closeMovie();
    }
    
    player.load(path);
    
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
            
        case OF_KEY_RIGHT:
            player.setFrame(player.getCurrentFrame() + fps * 5.0f);
            break;
            
        case OF_KEY_LEFT:
            player.setFrame(player.getCurrentFrame() - fps * 5.0f);
            break;
            
        default:
            break;
    }
}

void Player::draw() {
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

/*
void Player::draw(float x, float y) {
    ofSetColor(ofColor::black);
    ofDrawRectangle(x, y, width, height);
    
    ofSetColor(ofColor::white);
    player.draw(x, y, videoWidth, videoHeight);
}
 
 */



void Player::update() {
    if(player.isLoaded()) {
        player.update();
    }
    
}
