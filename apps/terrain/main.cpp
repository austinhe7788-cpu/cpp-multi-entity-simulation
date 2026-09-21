#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "TerrainAPI.h"

using namespace gis;

int main(int argc, char** argv) {
  if (argc < 6) {
    std::cout << "Usage: ./build/bin/terrain <output file prefix> <minLon> "
                 "<minLat> <maxLon> <maxLat>"
              << std::endl;
    std::cout << "Example: ./build/bin/terrain data/gis/umn -93.25000 44.96610 "
                 "-93.22080 44.97887"
              << std::endl;
    std::cout << "Example: ./build/bin/terrain data/gis/iterlaken 7.80939 "
                 "46.65674 7.94415 46.70779"
              << std::endl;
    std::cout << "Example: ./build/bin/terrain data/gis/upheaval-dome "
                 "-109.9697 38.42492 -109.91306 38.45406"
              << std::endl;
    std::cout << "Example: ./build/bin/terrain data/gis/yosemite-valley "
                 "-119.63047 37.71221 -119.51717 37.77105"
              << std::endl;

    return 1;
  }

  std::string pathPrefix = argv[1];
  double minLon = std::stod(argv[2]);
  double minLat = std::stod(argv[3]);
  double maxLon = std::stod(argv[4]);
  double maxLat = std::stod(argv[5]);

  TerrainAPI api;
  Image* sat = api.CreateSatelliteImage(minLon, minLat, maxLon, maxLat);
  if (sat) {
    sat->saveAs(pathPrefix + "-sat.png");
    delete sat;
  }

  return 0;
}
