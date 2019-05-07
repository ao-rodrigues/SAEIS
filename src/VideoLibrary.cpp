#include "VideoLibrary.h"
#include "ofApp.h"

VideoLibrary::VideoLibrary() {
    ofBackground(ofColor::white);
    ofSetVerticalSync(true);
    
    // List directory paths & remove all that do not conform to the extension
    dir.listDir(dir_path);
    dir.allowExt("mov");
    dir.allowExt("mp4");
    dir.sort();
    
    if (dir.size() > 0) {
        videoPaths.reserve(dir.size());
        //videoNames.reserve(dir.size());
        for (size_t i = 0; i < dir.size(); i++) {
            string path = dir.getPath(i);
            
            //videoNames.push_back(path);
            videoPaths.push_back(path);
        }
        
        player = std::make_shared<Player>(50, 600, 480, 270);
        
        videoBrowser = std::make_unique<VideoBrowser>(50, 50, 500, 500, 200, 200, videoPaths, player);
    } else {
        exit(-1);
    }
}

void VideoLibrary::update() {
    player->update();
}

void VideoLibrary::draw() {
    videoBrowser->draw();
    
    player->draw();
}

void VideoLibrary::key_pressed(int key) {
    if (dir.size() > 0) {
        player->keyPress(key);
    }
}
