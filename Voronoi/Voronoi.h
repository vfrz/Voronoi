#pragma once

#include "DistanceType.h"
#include "VoronoiPoint.h"
#include "ProcessingType.h"
#include <opencv2/opencv.hpp>

class Voronoi
{
public:
    cv::Mat process(DistanceType distanceType, ProcessingType processingType, int width, int height, int randomPointCount);
private:
    cv::Mat processBruteforce(DistanceType distanceType, int width, int height, int randomPointCount);
    cv::Mat processSequential(DistanceType distanceType, int width, int height, int randomPointCount);
    float getDistance(DistanceType distanceType, VoronoiPoint &point, cv::Point &position);
    std::vector<VoronoiPoint> generateRandomPoints(int width, int height, int randomPointCount);
};
