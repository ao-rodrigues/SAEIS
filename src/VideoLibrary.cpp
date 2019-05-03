#include "VideoLibrary.h"
#include "ofApp.h"

VideoLibrary::VideoLibrary() {
    ofBackground(ofColor::white);
    ofSetVerticalSync(true);
    
    // List directory paths & remove all that do not conform to the extension
    dir.listDir(dir_path);
    dir.allowExt(video_ext);
    dir.sort();
    
    
    if (dir.size() > 0) {
        videoPaths.reserve(dir.size());
        for (size_t i = 0; i < dir.size(); i++) {
            videoPaths.push_back(dir.getPath(i));
        }
        
        videoBrowser = std::make_unique<VideoBrowser>(VideoBrowser(50, 50, 200, 800, 200, 200, videoPaths));
        
        // Default player dimensions for now
        videoPlayer = std::make_unique<Player>(Player(videoPaths[0],
                                               ofApp::WINDOW_WIDTH / 2.0,
                                               PADDING,
                                               (ofApp::WINDOW_WIDTH / 2.0) - PADDING,
                                               ofApp::WINDOW_HEIGHT - PADDING));
    } else {
        exit(-1);
    }
}

void VideoLibrary::update() {
    videoBrowser->update();
    videoPlayer->update();
}

void VideoLibrary::draw() {
    videoBrowser->draw();
    
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
