#pragma once
#include "ofMain.h"

class Player {
    ofVideoPlayer player;
    string path;
    
    float playerWidth;
    float playerHeight;
    float playerXPosition;
    float playerYPosition;
    
    float videoWidth;
    float videoHeight;
    
    void play();
    void pause();
    void resume();
    void stop();
    
    
public:
    Player(string path, float xPos = 0, float yPos = 0, float width = 1024, float height = 768);
    void loadVideo(string path);
    void keyPress(int key);
    void draw();
    void update();
};
