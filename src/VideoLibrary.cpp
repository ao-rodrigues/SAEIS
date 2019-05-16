#include "VideoLibrary.h"
#include "ofApp.h"

VideoLibrary::VideoLibrary() {
    ofBackground(ofColor::white);
    ofSetVerticalSync(true);
    
    if(XML.loadFile(metadataFile)) {
        statusMsg = "metadata.xml loaded!";
    } else {
        statusMsg = "Unable to load metadata.xml. Check data/ folder.";
    }
    
    cout << statusMsg;
    
    // List directory paths & remove all that do not conform to the extension
    dir.listDir(dir_path);
    dir.allowExt("mov");
    dir.allowExt("mp4");
    dir.sortByDate();
    
    if(!XML.tagExists(lastProcessedTag)) {
        XML.addValue(lastProcessedTag, -1);
        XML.saveFile(metadataFile);
    }
    
    if (dir.size() > 0) {
        lastProcessedIdx = XML.getValue(lastProcessedTag, -1);
        
        if(lastProcessedIdx == dir.size() - 1) {
            videosProcessed = true;
        } else {
            videosProcessed = false;
        }
        
        learnFirstFrame = true;
        learnSecFrame = true;
        frameStepCounter = 0;
        
        vector<string> videoPaths;
        videoPaths.reserve(dir.size());
        
        videoNames.reserve(dir.size());
        for (size_t i = 0; i < dir.size(); i++) {
            string path = dir.getPath(i);
            string name = extractVideoName(path);
            
            videoNames.push_back(name);
            videoPaths.push_back(path);
        }
         
        tags.setup("Search by tag", "");
        tags.setPosition(600, 50);
        
        player = std::make_shared<Player>(50, 600, 480, 270);
        videoBrowser = std::make_unique<VideoBrowser>(50, 50, 500, 500, 200, 200, videoPaths, player);
        
        // Grab preview frames of already processed videos
        for(int i = 0; i < lastProcessedIdx; i++) {
            string vidName = videoNames[i];
            vector<ofTexture> frames;
            frames.reserve(NUM_PREVIEW_FRAMES);
            
            for(size_t j = 0; j < NUM_PREVIEW_FRAMES; j++){
                string framePath = previewFramesPath + vidName + "/" + to_string(j) + ".jpg";
                ofLoadImage(frames[j], framePath);
            }
            
            videoBrowser->updatePreviewFrames(dir.getPath(i), frames);
        }
        
    } else {
        exit(-1);
    }
}

bool VideoLibrary::absDiffsCompare(const DiffFrame &i, const DiffFrame &j){
    return i.absDiff < j.absDiff;
}

bool VideoLibrary::originalIdxCompare(const DiffFrame &i, const DiffFrame &j) {
    return i.originalIdx < j.originalIdx;
}

void VideoLibrary::update() {
    if(!videosProcessed){
        if(!hiddenPlayer.isLoaded()){
            hiddenPlayer.load(dir.getPath(lastProcessedIdx + 1));
            hiddenPlayer.play();
            hiddenPlayer.setVolume(0.0);
            hiddenPlayer.setLoopState(OF_LOOP_NONE);
        }
        
        hiddenPlayer.update();
        if(hiddenPlayer.isFrameNew() && (frameStepCounter % 10) == 0){
            if(learnFirstFrame) {
                ofxCvColorImage firstFrame;
                firstFrame.setFromPixels(hiddenPlayer.getPixels());
                framePair.first = firstFrame;
                
                learnFirstFrame = false;
            } else if (learnSecFrame) {
                ofxCvColorImage secFrame;
                secFrame.setFromPixels(hiddenPlayer.getPixels());
                framePair.second = secFrame;
                
                // Now we have a pair of frames
                processFrames(framePair.first, framePair.second);
                
                learnFirstFrame = true;
                learnSecFrame = true;
            }
        }
        
        if(hiddenPlayer.isFrameNew() && learnFirstFrame){
            frameStepCounter++;
        }
        
        // Check if we finished processing the video
        if(hiddenPlayer.getIsMovieDone()) {
            // Sort by diff descending
            sort(diffs.rbegin(), diffs.rend(), VideoLibrary::absDiffsCompare);
            
            // Get top 10 diffs
            vector<DiffFrame> topDiffs (diffs.begin(), diffs.begin() + NUM_PREVIEW_FRAMES);
            
            // Sort by original index to get original order
            sort(topDiffs.begin(), topDiffs.end(), VideoLibrary::originalIdxCompare);
            
            vector<ofTexture> previewFrames;
            previewFrames.reserve(topDiffs.size());
            
            for(int i = 0; i < topDiffs.size(); i++) {
                ofTexture tex = topDiffs[i].tex;
                
                previewFrames.push_back(tex);
                
                ofPixels texPixels;
                tex.readToPixels(texPixels);
                
                string vidName = videoNames[lastProcessedIdx + 1];
                string saveToPath = previewFramesPath + vidName + "/" + to_string(i) + ".jpg";
                
                // Save image for fast access after video is processed
                ofSaveImage(texPixels, saveToPath);
            }
            
            videoBrowser->updatePreviewFrames(dir.getPath(lastProcessedIdx + 1), previewFrames);
            
            diffs.clear();
            frameStepCounter = 0;
            hiddenPlayer.closeMovie();
            
            lastProcessedIdx++;
            XML.setValue(lastProcessedTag, lastProcessedIdx);
            XML.saveFile(metadataFile);
        }
        
        // Check if we processed all videos
        if(lastProcessedIdx == dir.size() - 1) {
            videosProcessed = true;
        }
    }
    
    
    player->update();
}

void VideoLibrary::draw() {
    tags.draw();
    videoBrowser->draw();
    player->draw();
}

void VideoLibrary::key_pressed(int key) {
    if (dir.size() > 0) {
        player->keyPress(key);
    }
}

string VideoLibrary::extractVideoName(string path) {
    string baseFilename = path.substr(path.find_last_of("/\\") + 1);
    std::size_t extStart = baseFilename.find_last_of(".");
    
    return baseFilename.substr(0, extStart);
}

void VideoLibrary::processFrames(ofxCvColorImage first, ofxCvColorImage second) {
    ofxCvGrayscaleImage grayFirst;
    ofxCvGrayscaleImage graySecond;
    //ofxCvGrayscaleImage grayDiff;
    
    vector<int> histFirst (256, 0);
    vector<int> histSecond (256, 0);
    
    grayFirst = first;
    graySecond = second;
    
    ofPixels & pixelsFirst = grayFirst.getPixels();
    ofPixels & pixelsSecond = graySecond.getPixels();
    
    for(int i = 0; i < pixelsFirst.size(); i++) {
        // pixelsFirst and pixelsSecond will have the same size
        histFirst[pixelsFirst[i]]++;
        histSecond[pixelsSecond[i]]++;
    }
    
    int absDiff = compareHistograms(histFirst, histSecond);
    
    ofTexture texSecond;
    texSecond.loadData(second.getPixels());
    
    DiffFrame df;
    df.absDiff = absDiff;
    df.tex = texSecond;
    df.originalIdx = (int)diffs.size();
    
    diffs.push_back(df);
}

int VideoLibrary::compareHistograms(vector<int> first, vector<int> second) {
    int absDiff = 0;
    
    for(int i = 0; i < first.size(); i++) {
        absDiff += abs(second[i] - first[i]);
    }
    
    return absDiff;
}

