#include "VoronoiPoint.h"

VoronoiPoint::VoronoiPoint(cv::Point position, cv::Vec3b color)
{
    _position = position;
    _color = color;
}

cv::Point &VoronoiPoint::getPosition()
{
    return _position;
}

cv::Vec3b &VoronoiPoint::getColor()
{
    return _color;
}
