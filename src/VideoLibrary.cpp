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
        
        colorProcessed = false;
        
        sumAvgLuminances = 0.0f;
        sumNumFaces = 0;
        
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
        for(int i = 0; i <= lastProcessedIdx; i++) {
            string vidName = videoNames[i];
            vector<ofTexture> frames;
            frames.resize(NUM_PREVIEW_FRAMES);
            
            for(int j = 0; j < NUM_PREVIEW_FRAMES; j++){
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
        statusMsg = PROCESSING_VIDEOS_MSG;
        
        if(!hiddenPlayer.isLoaded()){
            hiddenPlayer.load(dir.getPath(lastProcessedIdx + 1));
            hiddenPlayer.play();
            hiddenPlayer.setVolume(0.0);
            hiddenPlayer.setLoopState(OF_LOOP_NONE);
            
            XML.pushTag(metadataTag);
            XML.addTag(videoNames[lastProcessedIdx + 1]);
            XML.popTag();
            XML.saveFile(metadataFile);
        }
        
        hiddenPlayer.update();
        if(hiddenPlayer.isFrameNew() && (frameStepCounter % frameStep) == 0){
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
            float numFrames = (float)diffs.size();
            
            float finalLuminance = sumAvgLuminances / numFrames;
            float avgNumFaces = (float)sumNumFaces / numFrames;
            
            // Save values in XML
            XML.pushTag(metadataTag);
            XML.pushTag(videoNames[lastProcessedIdx + 1]);
            
            XML.addValue("LUMINANCE", finalLuminance);
            XML.addValue("NUM-FACES", avgNumFaces);
            XML.saveFile(metadataFile);
            
            XML.popTag();
            XML.popTag();
            
            // Sort by diff descending
            sort(diffs.rbegin(), diffs.rend(), VideoLibrary::absDiffsCompare);
            
            // Get top 10 diffs
            vector<DiffFrame> topDiffs (diffs.begin(), diffs.begin() + min((int)diffs.size(), NUM_PREVIEW_FRAMES));
            
            // Sort by original index to get original order
            sort(topDiffs.begin(), topDiffs.end(), VideoLibrary::originalIdxCompare);
            
            vector<ofTexture> previewFrames;
            //previewFrames.reserve(topDiffs.size());
            
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
            
            // Reset everything
            framePair.first.clear();
            framePair.second.clear();
            
            diffs.clear();
            frameStepCounter = 0;
            hiddenPlayer.closeMovie();
            
            colorProcessed = false;
            
            sumAvgLuminances = 0.0f;
            sumNumFaces = 0;
            
            learnFirstFrame = true;
            learnSecFrame = true;
            
            lastProcessedIdx++;
            XML.setValue(lastProcessedTag, lastProcessedIdx);
            XML.saveFile(metadataFile);
        }
        
        // Check if we processed all videos
        if(lastProcessedIdx == dir.size() - 1) {
            videosProcessed = true;
            statusMsg = VIDEOS_PROCESSED_MSG;
        }
    } else {
        statusMsg = VIDEOS_PROCESSED_MSG;
    }
    
    
    player->update();
}

void VideoLibrary::draw() {
    tags.draw();
    videoBrowser->draw();
    player->draw();
    
    ofSetColor(ofColor::black);
    ofDrawBitmapString(statusMsg, 600, 100);
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
    if(!colorProcessed) {
        getColorFirstMoment(first);
        colorProcessed = true;
    }
    
    ofxCvGrayscaleImage grayFirst;
    ofxCvGrayscaleImage graySecond;
    
    vector<int> histFirst (256, 0);
    vector<int> histSecond (256, 0);
    
    grayFirst = first;
    graySecond = second;
    
    ofPixels & pixelsFirst = grayFirst.getPixels();
    ofPixels & pixelsSecond = graySecond.getPixels();
    
    
    ofPixels & colorPixelsFirst = first.getPixels();
    int nChannels = (int)colorPixelsFirst.getNumChannels();
    
    int w = (int)pixelsFirst.getWidth();
    int h = (int)pixelsFirst.getHeight();
    
    ofxCvHaarFinder finder;
    finder.setup(haarCascadeXmlPath);
    ofImage ofImageFirst;
    
    ofImageFirst.allocate(w, h, OF_IMAGE_COLOR);
    ofImageFirst.setFromPixels(colorPixelsFirst);
    
    // Find number of faces in current frame
    finder.findHaarObjects(ofImageFirst);
    sumNumFaces += finder.blobs.size();
    
    float sumLuminances = 0.0f;
    
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            histFirst[pixelsFirst[j * w + i]]++;
            histSecond[pixelsSecond[j * w + i]]++;
            
            unsigned char r = colorPixelsFirst[(j * w + i) * nChannels];
            unsigned char g = colorPixelsFirst[(j * w + i) * nChannels + 1];
            unsigned char b = colorPixelsFirst[(j * w + i) * nChannels + 2];
            
            float pixelLuminance = 0.2125 * r + 0.7154 * g + 0.0721 * b;
            sumLuminances += pixelLuminance;
        }
    }
    
    sumAvgLuminances += sumLuminances / (w * h);
    
    int absDiff = compareHistograms(histFirst, histSecond);
    
    ofTexture texSecond;
    texSecond.loadData(second.getPixels());
    
    DiffFrame df;
    df.absDiff = absDiff;
    df.tex = texSecond;
    df.originalIdx = (int)diffs.size();
    
    diffs.push_back(df);
    
    
}

void VideoLibrary::getColorFirstMoment(ofxCvColorImage frame) {
    vector<int> redHist (256, 0);
    vector<int> greenHist (256, 0);
    vector<int> blueHist (256, 0);
    
    ofPixels & pixels = frame.getPixels();
    
    
    for(auto & pixel : pixels.getPixelsIter()) {
        ofColor pixelColor = pixel.getColor();
        
        redHist[pixelColor.r]++;
        greenHist[pixelColor.g]++;
        blueHist[pixelColor.b]++;
    }
    
    // Get the top values in every channel
    int maxRed = (int)distance(redHist.begin(), max_element(redHist.begin(), redHist.end()));
    int maxGreen = (int)distance(greenHist.begin(), max_element(greenHist.begin(), greenHist.end()));
    int maxBlue = (int)distance(blueHist.begin(), max_element(blueHist.begin(), blueHist.end()));
    
    XML.pushTag(metadataTag);
    XML.pushTag(videoNames[lastProcessedIdx + 1]);
    int tagNum = XML.addTag("COLOR");
    
    // Save values in XML file
    XML.setValue("COLOR:R", maxRed, tagNum);
    XML.setValue("COLOR:G", maxGreen, tagNum);
    XML.setValue("COLOR:B", maxBlue, tagNum);
    
    XML.popTag();
    XML.popTag();
    XML.saveFile(metadataFile);
    
}

int VideoLibrary::compareHistograms(vector<int> first, vector<int> second) {
    int absDiff = 0;
    
    for(int i = 0; i < first.size(); i++) {
        absDiff += abs(second[i] - first[i]);
    }
    
    return absDiff;
}

