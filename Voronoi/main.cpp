#include <iostream>
#include "Voronoi.h"

int main(int argc, char *argv[])
{
    if (argc < 6)
    {
        std::cout << "Missing arguments" << std::endl;
        return 1;
    }

    auto distanceType = (DistanceType) std::stoi(argv[1]);
    auto processingType = (ProcessingType) std::stoi(argv[2]);
    auto width = std::stoi(argv[3]);
    auto height = std::stoi(argv[4]);
    auto randomPointCount = std::stoi(argv[5]);

    auto voronoi = Voronoi();
    auto result = voronoi.process(distanceType, processingType, width, height, randomPointCount);

    cv::imshow("display window", result);
    while(cv::waitKey(1) != 27);

    return 0;
}
