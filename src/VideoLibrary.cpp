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
    
    //std::vector<string> videoNames;
    
    
    if (dir.size() > 0) {
        videoPaths.reserve(dir.size());
        //videoNames.reserve(dir.size());
        for (size_t i = 0; i < dir.size(); i++) {
            string path = dir.getPath(i);
            
            //videoNames.push_back(path);
            videoPaths.push_back(path);
        }
        
        player = std::make_shared<ofVideoPlayer>();
        
        videoBrowser = std::make_unique<VideoBrowser>(50, 50, 500, 500, 200, 200, videoPaths, player);
        
        // Default player dimensions for now
        videoPlayer = std::make_unique<Player>(videoPaths[0],
                                               ofApp::WINDOW_WIDTH / 2.0,
                                               PADDING,
                                               (ofApp::WINDOW_WIDTH / 2.0) - PADDING,
                                               ofApp::WINDOW_HEIGHT - PADDING);
    } else {
        exit(-1);
    }
}

void VideoLibrary::update() {
    //videoBrowser->update();
    //videoPlayer->update();
    
    if(player->isLoaded()) {
        player->update();
    }
}

void VideoLibrary::draw() {
    videoBrowser->draw();
    
    if(player->isLoaded()) {
        ofSetColor(ofColor::white);
        player->draw(50, 600, 200, 200);
    } else {
        ofSetColor(ofColor::gray);
        ofDrawRectangle(50, 600, 200, 200);
    }
    
    
    //videoPlayer->draw();
    
    //ofSetColor(ofColor::gray);
    
    /*
    for (size_t i = 0; i < dir.size(); i++) {
        if (i == currentVideoIdx) {
            ofSetColor(ofColor::red);
        } else {
            ofSetColor(ofColor::black);
        }
        
        auto const file_info = "file " + ofToString(i + 1) + ": " + dir.getName(i);
        ofDrawBitmapString(file_info, 50, i * 20 + 50);
    }
     */
}

/*
string VideoLibrary::extractVideoName(string path) {
    string baseFilename = path.substr(path.find_last_of("/\\") + 1);
    std::size_t extStart = baseFilename.find_last_of(".");
    
    return baseFilename.substr(0, extStart);
    
}
 */

void VideoLibrary::key_pressed(int key) {
    
    /*
    if (dir.size() > 0) {
        switch (key) {
            case OF_KEY_UP:
                currentVideoIdx--;
                videoPlayer->loadVideo(videoPaths[currentVideoIdx]);
                break;
            case OF_KEY_DOWN:
                currentVideoIdx++;
                videoPlayer->loadVideo(videoPaths[currentVideoIdx]);
                break;
            default:
                videoPlayer->keyPress(key);
                break;
        }
        
        currentVideoIdx %= dir.size();
    }
     */
}
