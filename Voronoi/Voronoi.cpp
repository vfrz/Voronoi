#include "Voronoi.h"

Voronoi::Voronoi(DistanceType distanceType, int width, int height, int randomPointCount)
{
    _distanceType = distanceType;
    _width = width;
    _height = height;
    _randomPointCount = randomPointCount;
}

void Voronoi::Process()
{
    _result = cv::Mat(_height, _width, CV_8UC3);
    auto randomPoints = generateRandomPoints();

    for (int x = 0; x < _width; ++x)
    {
        for (auto y = 0; y < _height; y++)
        {
            auto shortest = std::numeric_limits<float>::max();
            auto shortestPoint = randomPoints[0];

            auto p = cv::Point(x, y);

            for (auto point : randomPoints) {
                auto distance = getDistance(point, p);
                if (distance < shortest) {
                    shortest = distance;
                    shortestPoint = point;
                }
            }

            _result.at<cv::Vec3b>(y,x) = shortestPoint.getColor();

            //_result.at<cv::Vec3i>(p) = cv::Vec3i(color.getRed(), color.getGreen(), color.getBlue());
        }
    }
}

void Voronoi::Display()
{
    cv::imshow("Display window", _result);
    while(cv::waitKey(1) != 27);
}

std::vector<VoronoiPoint> Voronoi::generateRandomPoints()
{
    auto result = std::vector<VoronoiPoint>();

    for (auto i = 0; i < _randomPointCount; i++)
    {
        auto x = std::rand() % _width;
        auto y = std::rand() % _height;
        auto r = std::rand() % 255;
        auto g = std::rand() % 255;
        auto b = std::rand() % 255;
        auto point = VoronoiPoint(cv::Point(x, y), cv::Vec3b(r, g, b));
        result.emplace_back(point);
    }

    return result;
}

float Voronoi::getDistance(VoronoiPoint &point, cv::Point &position)
{
    auto pp = point.getPosition();

    switch (_distanceType)
    {
        case Euclidean:
            return std::sqrt((pp.x - position.x) * (pp.x - position.x) + (pp.y - position.y) * (pp.y - position.y));
        case Manhattan:
            return std::abs(pp.x - position.x) + std::abs(pp.y - position.y);
        case Chebyshev:
            return std::max(std::abs(pp.x - position.x), std::abs(pp.y - position.y));
        default:
            throw std::exception();
    }
}
