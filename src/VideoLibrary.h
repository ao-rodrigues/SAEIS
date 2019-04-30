#pragma once

#include "ofMain.h"
#include "Player.hpp"

class VideoLibrary {
    const string dir_path = "videos/";
    const string video_ext = ".mov";
    const float PADDING = 50.0;
    
    int currentVideoIdx = 0;
    
    ofDirectory dir;
    std::vector<string> videoPaths;
    //std::unique_ptr<Player> videoPlayer;
    Player *videoPlayer;
    
public:
    VideoLibrary();
    ~VideoLibrary() {
        delete videoPlayer;
    }
    void update();
    void draw();
    void key_pressed(int key);
};

