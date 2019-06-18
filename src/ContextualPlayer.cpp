#include "ContextualPlayer.h"
#include "ObjectDetection.h"
#include <opencv2/imgproc/imgproc.hpp>

ContextualPlayer::ContextualPlayer(string behavioursXml) {
    if(behavioursXML.loadFile(behavioursXml)) {
        cout << "Behaviours XML loaded!\n";
    } else {
        cout << "Behaviours XML not loaded!\n";
    }
    
    ofSetVerticalSync(true);
    
    camera.setDeviceID(0);
    camera.setDesiredFrameRate(30);
    camera.initGrabber(CAMERA_WIDTH, CAMERA_HEIGHT);
    
    player.setLoopState(OF_LOOP_NORMAL);
    isFullscreen = false;
    objDetected = false;
    objCoords = vector<cv::Point2f>(4);
    
    videoAreaXPos = ofGetWindowWidth() - PLAYER_WIDTH - 50;
    videoAreaYPos = 50;
    
    videoAreaWidth = PLAYER_WIDTH;
    videoAreaHeight = PLAYER_HEIGHT;
    
    loadVideo("DEFAULT");
    videoStart = ofGetElapsedTimeMillis();
    
    faceFinder.setup(HAAR_CASCADE);
}

void ContextualPlayer::update() {
    camX = ofGetWindowWidth() - camera.getWidth() - 50;
    camY = ofGetWindowHeight() - camera.getHeight() - 50;
    
    camera.update();
    
    if(camera.isFrameNew()) {
        processFrame(camera.getPixels());
    }
    
    player.update();
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
        camera.draw(camX, camY);
        
        ofNoFill();
        for(unsigned int i = 0; i < faceFinder.blobs.size(); i++) {
            ofRectangle cur = faceFinder.blobs[i].boundingRect;
            
            ofSetColor(ofColor::green);
            ofDrawRectangle(camX + cur.x, camY + cur.y, cur.width, cur.height);
        }
        
        ofFill();
        
        if(objDetected) {
            ofNoFill();
            
            ofSetColor(ofColor::blue);
            
            ofDrawLine(objCoords[0].x, objCoords[0].y, objCoords[1].x, objCoords[1].y);
            ofDrawLine(objCoords[1].x, objCoords[1].y, objCoords[2].x, objCoords[2].y);
            ofDrawLine(objCoords[2].x, objCoords[2].y, objCoords[3].x, objCoords[3].y);
            ofDrawLine(objCoords[3].x, objCoords[3].y, objCoords[0].x, objCoords[0].y);
            
            ofFill();
        }
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

void ContextualPlayer::loadVideo(const string videoTag) {
    string path = getRandomVideoPath(videoTag);
    
    if(!path.empty()) {
        if (player.isLoaded()) {
            player.closeMovie();
        }
        
        player.load(path);
        calculatePlayerDimensions();
        
        player.play();
        videoStart = ofGetElapsedTimeMillis();
    }

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
    detectObject(pixels);
    faceFinder.findHaarObjects(pixels);
    
    uint64_t currTime = ofGetElapsedTimeMillis();
    
    if(currTime - videoStart > VIDEO_PLAY_TIME_MILLIS) {
        // The video has played for 5 seconds, we can process the camera again
        objDetected = false;
        
        int numFaces = (int)faceFinder.blobs.size();
        
        if(numFaces == 0) {
            loadVideo("DEFAULT");
        } else if(numFaces <= LOW_NUM_FACES) {
            loadVideo("LOW-NUM-FACES");
        } else if(numFaces <= MED_NUM_FACES) {
            loadVideo("MED-NUM-FACES");
        } else if(numFaces > MED_NUM_FACES) {
            loadVideo("HIGH-NUM-FACES");
        }
    }
}

void ContextualPlayer::detectObject(ofPixels &scenePixels) {
    ofxCvColorImage ofSceneColor;
    ofSceneColor.allocate((int)scenePixels.getWidth(), (int)scenePixels.getHeight());
    ofSceneColor.setFromPixels(scenePixels);
    cv::Mat sceneMat = cv::cvarrToMat(ofSceneColor.getCvImage());
    
    ofImage ofImgRef;
    ofImgRef.load(IMG_REF);
    
    ofxCvColorImage ofxCvImgRef;
    ofxCvImgRef.allocate((int)ofImgRef.getWidth(), (int)ofImgRef.getHeight());
    
    ofxCvImgRef.setFromPixels(ofImgRef.getPixels());
    
    cv::Mat imgRef = cv::cvarrToMat(ofxCvImgRef.getCvImage());
    cv::Mat objMat;
    cv::cvtColor(imgRef, objMat, CV_BGR2GRAY);
    
    // This function returns either a vector with the 4 corners of the object in the image, or nothing if no object is found
    vector<cv::Point2f> objCorners = ObjectDetection::findObjectInScene(objMat, sceneMat);
    
    if(!objCorners.empty()) {
        setupObjectRect(objCorners);
        
        uint64_t currTime = ofGetElapsedTimeMillis();
        if(currTime - videoStart > VIDEO_PLAY_TIME_MILLIS) {
            // The video has played for 5 seconds, we can process the camera again
            loadVideo("COCA-COLA");
        }
    }
}

void ContextualPlayer::setupObjectRect(vector<cv::Point2f> objCorners) {
    cv::Point2f camCoords ((float)camX, (float)camY);
    
    for(int i = 0; i < objCorners.size(); i++) {
        objCoords[i] = objCorners[i] + camCoords;
    }
    
    objDetected = true;
}


string ContextualPlayer::getRandomVideoPath(string tag) {
    behavioursXML.pushTag("BEHAVIOURS");
    behavioursXML.pushTag(tag);
    
    int numTags = behavioursXML.getNumTags("VIDEO");
    
    int i = rand() % numTags;
    
    string path = behavioursXML.getValue("VIDEO", "", i);
    
    behavioursXML.popTag();
    behavioursXML.popTag();
    
    return path;
}
