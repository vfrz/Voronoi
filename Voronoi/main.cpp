#include <iostream>
#include "DistanceType.h"
#include "Voronoi.h"

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        std::cout << "Missing arguments" << std::endl;
        return 1;
    }

    auto distanceType = (DistanceType) std::stoi(argv[1]);
    auto width = std::stoi(argv[2]);
    auto height = std::stoi(argv[3]);
    auto randomPointCount = std::stoi(argv[4]);

    auto voronoi = Voronoi(distanceType, width, height, randomPointCount);
    voronoi.Process();
    voronoi.Display();

    return 0;
}
