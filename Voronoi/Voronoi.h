#pragma once

#include "DistanceType.h"
#include "VoronoiPoint.h"
#include <opencv2/opencv.hpp>

class Voronoi
{
public:
    Voronoi(DistanceType distanceType, int width, int height, int randomPointCount);
    void Process();
    void Display();
private:
    float getDistance(VoronoiPoint &point, cv::Point &position);
    std::vector<VoronoiPoint> generateRandomPoints();
    DistanceType _distanceType;
    int _width;
    int _height;
    int _randomPointCount;
    cv::Mat _result;
};
