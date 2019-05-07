#pragma once

#include "ofMain.h"
#include "Player.h"
#include "VideoBrowser.h"

class VideoLibrary {
    const string dir_path = "videos/";
    const string video_ext = ".mov";
    const float PADDING = 50.0;
    
    int currentVideoIdx = 0;
    
    ofDirectory dir;
    std::vector<string> videoPaths;
    std::shared_ptr<Player> player;
    
    std::unique_ptr<VideoBrowser> videoBrowser;
    
public:
    VideoLibrary();
    void update();
    void draw();
    void key_pressed(int key);
};

