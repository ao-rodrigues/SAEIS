#ifndef ObjectDetection_h
#define ObjectDetection_h


#endif /* ObjectDetection_h */

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>

namespace ObjectDetection {
    const float MATCH_RATIO_THRESH = 0.7f;
    const int MIN_MATCH_COUNT = 13;
    
    std::vector<cv::DMatch> computeMatches(cv::Mat obj, cv::Mat scene);
    
    std::vector<cv::Point2f> findObjectInScene(cv::Mat objMat, cv::Mat sceneMat);
}
