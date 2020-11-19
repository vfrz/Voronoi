cmake_minimum_required(VERSION 3.0)
project(Voronoi)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OPENCV_INCLUDE_DIRS})

add_executable(Voronoi main.cpp DistanceType.h Voronoi.cpp Voronoi.h VoronoiPoint.cpp VoronoiPoint.h ProcessingType.h)

target_link_libraries(Voronoi ${OpenCV_LIBS})