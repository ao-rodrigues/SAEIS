#include "VideoLibrary.h"
#include "ofApp.h"
#include <opencv2/imgproc/imgproc.hpp>
#include "helper_functions.h"

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
        sumVerPixels = 0;
        sumHorPixels = 0;
        sumDia45Pixels = 0;
        sumDia135Pixels = 0;
        
        learnFirstFrame = true;
        learnSecFrame = true;
        frameStepCounter = 0;
        
        vector<string> videoPaths;
        videoPaths.reserve(dir.size());
        
        videoNames.reserve(dir.size());
        for (size_t i = 0; i < dir.size(); i++) {
            string path = dir.getPath(i);
            string name = helper_functions::extractVideoName(path);
            
            videoNames.push_back(name);
            videoPaths.push_back(path);
        }
         
        tagSearchInput.setup("Search by tag", "");
        tagSearchInput.setPosition(600, 50);
        tagSearchInput.addListener(this, &VideoLibrary::tagSearchInputSubmit);
        
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
        
        videoBrowser->setupKeywordsDisplay(550, 610, metadataFile);
        
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
            
            XML.pushTag(metadataTag);
            XML.addTag(videoNames[lastProcessedIdx + 1]);
            XML.pushTag(videoNames[lastProcessedIdx + 1]);
            XML.addTag("KEYWORDS");
            
            XML.popTag();
            XML.popTag();
            XML.saveFile(metadataFile);
        }
        
        statusMsg = PROCESSING_VIDEOS_MSG + to_string(lastProcessedIdx + 2) + "/" + to_string(videoNames.size());
        
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
        
        
        if(hiddenPlayer.isFrameNew()){
            frameStepCounter++;
        }
        
        // Check if we finished processing the video
        if(hiddenPlayer.getIsMovieDone()) {
            float numFrames = (float)diffs.size();
            
            float finalLuminance = sumAvgLuminances / numFrames;
            float avgNumFaces = (float)sumNumFaces / numFrames;
            
            float avgVerPixels = (float)sumVerPixels / numFrames;
            float avgHorPixels = (float)sumHorPixels / numFrames;
            float avgDia45Pixels = (float)sumDia45Pixels / numFrames;
            float avgDia135Pixels = (float)sumDia135Pixels / numFrames;
            
            int rythm = calculateRythm();
            string rythmLevel = "";
            
            if (rythm < 5) {
                rythmLevel = LOW_RYTHM;
            } else if (rythm >= 5 && rythm < 15) {
                rythmLevel = MED_RYTHM;
            } else if (rythm >= 15){
                rythmLevel = HIGH_RYTHM;
            }
            
            // Save values in XML
            XML.pushTag(metadataTag);
            XML.pushTag(videoNames[lastProcessedIdx + 1]);
            
            XML.addValue("LUMINANCE", finalLuminance);
            XML.addValue("NUM-FACES", avgNumFaces);
            XML.addValue("RYTHM", rythmLevel);
            
            XML.addTag("EDGE-DIST");
            XML.pushTag("EDGE-DIST");
            
            XML.addValue("VERTICAL", avgVerPixels);
            XML.addValue("HORIZONTAL", avgHorPixels);
            XML.addValue("DIA-45", avgDia45Pixels);
            XML.addValue("DIA-135", avgDia135Pixels);
            
            XML.popTag();
            
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
            sumVerPixels = 0;
            sumHorPixels = 0;
            sumDia45Pixels = 0;
            sumDia135Pixels = 0;
            
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
    tagSearchInput.draw();
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

void VideoLibrary::processEdgeDistribution(ofxCvColorImage colorImg) {
    cv::Mat colorMat = cv::cvarrToMat(colorImg.getCvImage());
    cv::Mat greyMat;
    cv::cvtColor(colorMat, greyMat, CV_BGR2GRAY);
    
    // Create filter kernels
    cv::Mat verEdgeFilter = (cv::Mat_<double>(2, 2) << 1, -1, 1, -1);
    cv::Mat horEdgeFilter = (cv::Mat_<double>(2, 2) << 1, 1, -1, -1);
    cv::Mat dia45EdgeFilter = (cv::Mat_<double>(2, 2) << sqrt(2), 0, 0, -1 * sqrt(2));
    cv::Mat dia135EdgeFilter = (cv::Mat_<double>(2, 2) << 0, sqrt(2), -1 * sqrt(2), 0);
    
    // Instantiate destination matrices
    cv::Mat verEdgeDest, horEdgeDest, dia45EdgeDest, dia135EdgeDest;
    
    cv::filter2D(greyMat, verEdgeDest, -1, verEdgeFilter);
    cv::filter2D(greyMat, horEdgeDest, -1, horEdgeFilter);
    cv::filter2D(greyMat, dia45EdgeDest, -1, dia45EdgeFilter);
    cv::filter2D(greyMat, dia135EdgeDest, -1, dia135EdgeFilter);
    
    // Count relevant pixels
    sumVerPixels += cv::countNonZero(verEdgeDest);
    sumHorPixels += cv::countNonZero(horEdgeDest);
    sumDia45Pixels += cv::countNonZero(dia45EdgeDest);
    sumDia135Pixels += cv::countNonZero(dia135EdgeDest);
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
    
    processEdgeDistribution(first);
    
}

void VideoLibrary::getColorFirstMoment(ofxCvColorImage frame) {
    /*
    vector<int> redHist (256, 0);
    vector<int> greenHist (256, 0);
    vector<int> blueHist (256, 0);
    */
    
    vector<int> hueHist (360, 0);
     
    //float sumHue = 0.0f;
    
    ofPixels & pixels = frame.getPixels();
    
    for(auto & pixel : pixels.getPixelsIter()) {
        ofColor pixelColor = pixel.getColor();
        hueHist[(int)pixelColor.getHueAngle()]++;
    }
    
    
    int maxHueCount = -1;
    int dominantHue = 0;
    
    for(int i = 0; i < hueHist.size(); i++) {
        if(hueHist[i] > maxHueCount) {
            maxHueCount = hueHist[i];
            dominantHue = i;
        }
    }
     
     
    XML.pushTag(metadataTag);
    XML.pushTag(videoNames[lastProcessedIdx + 1]);
    int tagNum = XML.addTag("COLOR");
    XML.setValue("COLOR:HUE", dominantHue, tagNum);
     
    XML.popTag();
    XML.popTag();
    XML.saveFile(metadataFile);
    
}

int VideoLibrary::calculateRythm() {
    int rythm = 0;
    int numPixels = diffs[0].tex.getWidth() * diffs[0].tex.getHeight();
    
    // Baseline a third the pixels in the image have changed
    float baseline = (float)numPixels / 3.0f;
    
    for(int i = 0; i < diffs.size(); i++) {
        if(diffs[i].absDiff >= baseline) {
            rythm++;
        }
    }
    
    return rythm;
}

int VideoLibrary::compareHistograms(vector<int> first, vector<int> second) {
    int absDiff = 0;
    
    for(int i = 0; i < first.size(); i++) {
        absDiff += abs(second[i] - first[i]);
    }
    
    return absDiff;
}

void VideoLibrary::tagSearchInputSubmit(string & input) {
    set<string> tags;
    helper_functions::split(input, ',', tags);
    
    XML.pushTag(metadataTag);
    for(int i = 0; i < videoNames.size(); i++) {
        XML.pushTag(videoNames[i]);
        XML.pushTag("KEYWORDS");
        
        int numKeywords = XML.getNumTags("KEYWORD");
        
        for(int j = 0; j < numKeywords; j++) {
            string keyword = XML.getValue("KEYWORD", "", j);
            bool setEnabled;
            
            if(tags.find(keyword) != tags.end()) {
                // This video has one of the tags received
                setEnabled = true;
            } else {
                setEnabled = false;
            }
            
            videoBrowser->setThumbnailEnabled(dir.getPath(i), setEnabled);
        }
        
        
    }
}

