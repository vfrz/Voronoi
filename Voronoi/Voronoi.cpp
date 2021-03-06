#include "Voronoi.h"

cv::Mat
Voronoi::process(DistanceType distanceType, ProcessingType processingType, int width, int height, int randomPointCount)
{
    switch (processingType)
    {
        case Bruteforce:
            return processBruteforce(distanceType, width, height, randomPointCount);
        case Sequential:
            return processSequential(width, height, randomPointCount);
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

            for (auto point : randomPoints)
            {
                auto distance = getDistance(distanceType, point, p);
                if (distance < shortest)
                {
                    shortest = distance;
                    shortestPoint = point;
                }
            }

            result.at<cv::Vec3b>(y, x) = shortestPoint.getColor();
        }
    }

    return result;
}

cv::Mat Voronoi::processSequential(int width, int height, int randomPointCount)
{
    auto result = cv::Mat(height, width, CV_8UC3);
    auto randomPoints = generateRandomPoints(width, height, randomPointCount);

    int distanceArray[width][height];
    std::optional<VoronoiPoint> voronoiPointArray[width][height];

    auto firstMask = new std::tuple<cv::Point, int>[12]{
            std::tuple<cv::Point, int>(cv::Point(-1, 0), 3),
            std::tuple<cv::Point, int>(cv::Point(0, -1), 3),
            std::tuple<cv::Point, int>(cv::Point(-1, -1), 4),
            std::tuple<cv::Point, int>(cv::Point(1, -1), 4),
            std::tuple<cv::Point, int>(cv::Point(-2, 0), 6),
            std::tuple<cv::Point, int>(cv::Point(0, -2), 6),
            std::tuple<cv::Point, int>(cv::Point(-2, -1), 7),
            std::tuple<cv::Point, int>(cv::Point(2, -1), 7),
            std::tuple<cv::Point, int>(cv::Point(-1, -2), 7),
            std::tuple<cv::Point, int>(cv::Point(1, -2), 7),
            std::tuple<cv::Point, int>(cv::Point(2, -2), 8),
            std::tuple<cv::Point, int>(cv::Point(-2, -2), 8)
    };

    auto secondMask = new std::tuple<cv::Point, int>[12];

    for (auto i = 0; i < 12; i++)
    {
        auto firstMaskValue = std::get<1>(firstMask[i]);
        auto firstMaskPoint = std::get<0>(firstMask[i]);
        secondMask[i] = std::tuple<cv::Point, int>(cv::Point(-firstMaskPoint.x, -firstMaskPoint.y), firstMaskValue);
    }

    // Fill the array with either 0 if current position is a point or int.MaxValue if not
    for (auto x = 0; x < width; x++)
    {
        for (auto y = 0; y < height; y++)
        {
            auto currentPosition = cv::Point(x, y);
            auto currentPoint = getPointAt(randomPoints, currentPosition);
            voronoiPointArray[x][y] = currentPoint;
            if (currentPoint.has_value())
            {
                distanceArray[x][y] = 0;
            } else
            {
                distanceArray[x][y] = std::numeric_limits<int>::max() - (std::max(width, height) * 2);
            }
        }
    }

    // First pass from top left to bottom right
    for (auto x = 2; x < width - 2; x++)
    {
        for (auto y = 2; y < height - 2; y++)
        {
            auto currentPosition = cv::Point(x, y);
            for (auto i = 0; i < 12; i++)
            {
                auto maskTuple = firstMask[i];
                auto offsetPosition = currentPosition + std::get<0>(maskTuple);

                if (distanceArray[x][y] + std::get<1>(maskTuple) < distanceArray[offsetPosition.x][offsetPosition.y])
                {
                    distanceArray[offsetPosition.x][offsetPosition.y] = distanceArray[x][y] + std::get<1>(maskTuple);
                    if (voronoiPointArray[x][y].has_value())
                    {
                        voronoiPointArray[offsetPosition.x][offsetPosition.y] = voronoiPointArray[x][y];
                    }
                }
            }
        }
    }

    // Second pass from bottom right to top left
    for (auto x = width - 3; x > 1; x--)
    {
        for (auto y = height - 3; y > 1; y--)
        {
            auto currentPosition = cv::Point(x, y);
            for (auto i = 0; i < 12; i++)
            {
                auto maskTuple = secondMask[i];
                auto offsetPosition = currentPosition + std::get<0>(maskTuple);

                if (distanceArray[x][y] + std::get<1>(maskTuple) < distanceArray[offsetPosition.x][offsetPosition.y])
                {
                    distanceArray[offsetPosition.x][offsetPosition.y] = distanceArray[x][y] + std::get<1>(maskTuple);
                    if (voronoiPointArray[x][y].has_value())
                    {
                        voronoiPointArray[offsetPosition.x][offsetPosition.y] = voronoiPointArray[x][y];
                    }
                }
            }
        }
    }

    // Fill the result Mat
    for (auto x = 0; x < width; x++)
    {
        for (auto y = 0; y < height; y++)
        {
            if (voronoiPointArray[x][y].has_value())
            {
                result.at<cv::Vec3b>(y, x) = voronoiPointArray[x][y]->getColor();
            }
        }
    }

    return result;
}

std::vector<VoronoiPoint> Voronoi::generateRandomPoints(int width, int height, int randomPointCount)
{
    auto result = std::vector<VoronoiPoint>();

    for (auto i = 0; i < randomPointCount; i++)
    {
        auto x = std::rand() % width;
        auto y = std::rand() % height;
        auto color = generateRandomColor(false);
        auto point = VoronoiPoint(cv::Point(x, y), color);
        result.emplace_back(point);
    }

    return result;
}

cv::Vec3b Voronoi::generateRandomColor(bool flatColors)
{
    if (flatColors)
    {
        auto colors = new cv::Vec3b[20]{
                cv::Vec3b(26, 188, 156),
                cv::Vec3b(46, 204, 113),
                cv::Vec3b(52, 152, 219),
                cv::Vec3b(155, 89, 182),
                cv::Vec3b(52, 73, 94),
                cv::Vec3b(22, 160, 133),
                cv::Vec3b(39, 174, 96),
                cv::Vec3b(41, 128, 185),
                cv::Vec3b(142, 68, 173),
                cv::Vec3b(44, 62, 80),
                cv::Vec3b(241, 196, 15),
                cv::Vec3b(230, 126, 34),
                cv::Vec3b(231, 76, 60),
                cv::Vec3b(236, 240, 241),
                cv::Vec3b(149, 165, 166),
                cv::Vec3b(243, 156, 18),
                cv::Vec3b(211, 84, 0),
                cv::Vec3b(192, 57, 43),
                cv::Vec3b(189, 195, 199),
                cv::Vec3b(127, 140, 141)
        };
        auto color = colors[rand() % 20];
        return cv::Vec3b(color[2], color[1], color[0]);
    }

    auto r = std::rand() % 255;
    auto g = std::rand() % 255;
    auto b = std::rand() % 255;
    return cv::Vec3b(b, g, r);
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

std::optional<VoronoiPoint> Voronoi::getPointAt(std::vector<VoronoiPoint> &vector, cv::Point &position)
{
    for (auto point : vector)
    {
        if (point.getPosition().x == position.x && point.getPosition().y == position.y)
        {
            return std::optional<VoronoiPoint>(point);
        }
    }
    return std::optional<VoronoiPoint>();
}
