#pragma once

#include <opencv2/opencv.hpp>

class VoronoiPoint
{
public:
    VoronoiPoint(cv::Point position, cv::Vec3b color);
    cv::Point &getPosition();
    cv::Vec3b &getColor();
private:
    cv::Point _position;
    cv::Vec3b _color;
};
