#include <stdio.h>
#include "ObjectDetection.h"

std::vector<cv::DMatch> ObjectDetection::computeMatches(cv::Mat obj, cv::Mat scene) {
    std::vector<cv::KeyPoint> keypointsObj, keypointsScene;
    cv::Mat descriptorsObj, descriptorsScene;
    
    cv::Ptr<cv::Feature2D> detector = cv::ORB::create(500);
    
    detector->detectAndCompute(obj, cv::noArray(), keypointsObj, descriptorsObj);
    detector->detectAndCompute(scene, cv::noArray(), keypointsScene, descriptorsScene);
    
    std::vector<cv::DMatch> goodMatches;
    
    // Check if there are any descriptors/ keypoints before proceeding
    if(!(descriptorsObj.empty() || descriptorsScene.empty() || keypointsObj.empty() || keypointsScene.empty())) {
        
        //cv::FlannBasedMatcher matcher = cv::FlannBasedMatcher(cv::makePtr<cv::flann::LshIndexParams>(6, 12, 1));
        cv::BFMatcher matcher(cv::NORM_L2);
        
        std::vector<std::vector<cv::DMatch> > knnMatches;
        matcher.knnMatch(descriptorsObj, descriptorsScene, knnMatches, 2);
        
        // Filter matches according to Lowe's ratio test
        for(int i = 0; i < knnMatches.size(); i++) {
            
            // Safety measure because not all matches have a correspondence using ORB and FLANN
            if(knnMatches[i].size() >= 2) {
                if(knnMatches[i][0].distance < MATCH_RATIO_THRESH * knnMatches[i][1].distance) {
                    goodMatches.push_back(knnMatches[i][0]);
                }
            }
        }
    }
    
    return goodMatches;
}

