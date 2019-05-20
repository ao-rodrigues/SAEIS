#pragma once
#include "ofMain.h"

class Player {
    ofVideoPlayer player;
    string vidPath;
    float xPos, yPos;
    float width, height;
    
    float videoXPos, videoYPos;
    float videoWidth, videoHeight;
    
    
    //void play();
    void pause();
    void resume();
    void stop();
    
public:
    Player();
    Player(float x, float y, float w, float h);
    
    void load(const string path);
    void keyPress(int key);
    
    void draw();
    
    void play();
    void update();
};
