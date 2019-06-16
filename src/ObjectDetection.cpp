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
    
    if(goodMatches.size() >= MIN_MATCH_COUNT) {
        return goodMatches;
    } else {
        return std::vector<cv::DMatch>();
    }
}

std::vector<cv::Point2f> ObjectDetection::findObjectInScene(cv::Mat objMat, cv::Mat sceneMat) {
    std::vector<cv::KeyPoint> keypointsObj, keypointsScene;
    cv::Mat descriptorsObj, descriptorsScene;
    
    cv::Ptr<cv::Feature2D> detector = cv::ORB::create(500);
    
    detector->detectAndCompute(objMat, cv::noArray(), keypointsObj, descriptorsObj);
    detector->detectAndCompute(sceneMat, cv::noArray(), keypointsScene, descriptorsScene);
    
    std::vector<cv::DMatch> goodMatches;
    
    // Check if there are any descriptors/ keypoints before proceeding
    if(!(descriptorsObj.empty() || descriptorsScene.empty() || keypointsObj.empty() || keypointsScene.empty())) {
        
        cv::FlannBasedMatcher matcher = cv::FlannBasedMatcher(cv::makePtr<cv::flann::LshIndexParams>(12, 20, 2));
        //cv::BFMatcher matcher(cv::NORM_L2);
        
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
    
    if(goodMatches.size() >= MIN_MATCH_COUNT) {
        std::vector<cv::Point2f> obj;
        std::vector<cv::Point2f> scene;
        
        for (int i = 0; i < goodMatches.size(); i++) {
            obj.push_back(keypointsObj[goodMatches[i].queryIdx].pt);
            scene.push_back(keypointsScene[goodMatches[i].trainIdx].pt);
        }
        
        cv::Mat h = cv::findHomography(obj, scene, cv::RANSAC);
        
        if(!h.empty()) {
            std::vector<cv::Point2f> objCorners(4);
            
            objCorners[0] = cv::Point2f(0, 0);
            objCorners[1] = cv::Point2f((float)objMat.cols, 0);
            objCorners[2] = cv::Point2f((float)objMat.cols, (float)objMat.rows);
            objCorners[3] = cv::Point2f(0, (float)objMat.rows);
            
            std::vector<cv::Point2f> sceneCorners(4);
            
            cv::perspectiveTransform(objCorners, sceneCorners, h);
            return sceneCorners;
        } else {
            return std::vector<cv::Point2f>();
        }
    } else {
        return std::vector<cv::Point2f>();
    }
}

