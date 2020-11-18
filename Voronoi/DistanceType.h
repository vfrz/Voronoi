#pragma once

enum DistanceType
{
    Euclidean = 0, // Sqrt((x1 - x2)²+(y1-y2)²)
    Manhattan = 1, // |x1-x2|+|y1-y2|
    Chebyshev = 2  // Max(|x1-x2|,|y1-y2|)
};
