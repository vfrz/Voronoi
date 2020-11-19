#include "Voronoi.h"

cv::Mat Voronoi::process(DistanceType distanceType, ProcessingType processingType, int width, int height, int randomPointCount)
{
    switch (processingType)
    {
        case Bruteforce:
            return processBruteforce(distanceType, width, height, randomPointCount);
        case Sequential:
            return processSequential(distanceType, width, height, randomPointCount);
        default:
            return cv::Mat(0, 0, CV_8UC3);
    }
}

cv::Mat Voronoi::processBruteforce(DistanceType distanceType, int width, int height, int randomPointCount)
{
    auto result = cv::Mat(height, width, CV_8UC3);
    auto randomPoints = generateRandomPoints(width, height, randomPointCount);

    for (int x = 0; x < width; ++x)
    {
        for (auto y = 0; y < height; y++)
        {
            auto shortest = std::numeric_limits<float>::max();
            auto shortestPoint = randomPoints[0];

            auto p = cv::Point(x, y);

            for (auto point : randomPoints) {
                auto distance = getDistance(distanceType, point, p);
                if (distance < shortest) {
                    shortest = distance;
                    shortestPoint = point;
                }
            }

            result.at<cv::Vec3b>(y,x) = shortestPoint.getColor();
        }
    }

    return result;
}

cv::Mat Voronoi::processSequential(DistanceType distanceType, int width, int height, int randomPointCount)
{
    return cv::Mat();
}

std::vector<VoronoiPoint> Voronoi::generateRandomPoints(int width, int height, int randomPointCount)
{
    auto result = std::vector<VoronoiPoint>();

    for (auto i = 0; i < randomPointCount; i++)
    {
        auto x = std::rand() % width;
        auto y = std::rand() % height;
        auto r = std::rand() % 255;
        auto g = std::rand() % 255;
        auto b = std::rand() % 255;
        auto point = VoronoiPoint(cv::Point(x, y), cv::Vec3b(r, g, b));
        result.emplace_back(point);
    }

    return result;
}

float Voronoi::getDistance(DistanceType distanceType, VoronoiPoint &point, cv::Point &position)
{
    auto pp = point.getPosition();

    switch (distanceType)
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
