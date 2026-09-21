#include "TerrainAPI.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Image.h"
#include "WebService.h"

namespace gis {

double longitudeToMercator(double longitude) {
  return (180.0 + longitude) / 360.0;
}

double latitudeToMercator(double latitude) {
  return (
      1.0 / (2.0 * M_PI) *
      (M_PI - std::log(std::tan(M_PI / 4 + latitude * M_PI / 180.0 / 2.0))));
}

int longitudeToTile(double longitude, int zoom) {
  return (int)std::floor(std::pow(2, zoom) * longitudeToMercator(longitude));
}

int latitudeToTile(double latitude, int zoom) {
  return (int)std::floor(std::pow(2, zoom) * latitudeToMercator(latitude));
}

float mercatorToLongitude(float mercator) { return 360.0 * mercator - 180; }

float mercatorToLatitude(float mercator) {
  float x =
      2.0 * (std::atan(std::exp(-2.0 * M_PI * mercator + M_PI)) - M_PI / 4.0);
  return x * 180 / M_PI;
}

float tileToLongitude(int tile, int zoom) {
  float mercator = 1.0 * tile / std::pow(2, zoom);
  return mercatorToLongitude(mercator);
}

float tileToLatitude(int tile, int zoom) {
  float mercator = 1.0 * tile / std::pow(2, zoom);
  return mercatorToLatitude(mercator);
}

Image* TerrainAPI::CreateSatelliteImage(float minLon, float minLat,
                                        float maxLon, float maxLat) {
  WebService ws(
      "https://server.arcgisonline.com/arcgis/rest/services/World_Imagery/"
      "MapServer/tile/");

  double minX = longitudeToMercator(minLon);
  double maxX = longitudeToMercator(maxLon);
  double minY = latitudeToMercator(minLat);
  double maxY = latitudeToMercator(maxLat);

  std::cout << mercatorToLatitude(maxY) << std::endl;

  int xZoom = std::log(4.0 / std::abs(maxX - minX)) / std::log(2.0);
  std::cout << xZoom << std::endl;

  int yZoom = std::log(4.0 / std::abs(maxY - minY)) / std::log(2.0);

  int zoom = std::min(xZoom, yZoom);

  int minXTile = longitudeToTile(minLon, zoom);
  int maxXTile = longitudeToTile(maxLon, zoom);
  int minYTile = latitudeToTile(minLat, zoom);
  int maxYTile = latitudeToTile(maxLat, zoom);

  int width = (std::abs(maxXTile - minXTile) + 1) * 256;
  int height = (std::abs(maxYTile - minYTile) + 1) * 256;

  std::cout << width << "x" << height << std::endl;

  Image combined(width, height);
  for (int tileX = minXTile; tileX <= maxXTile; tileX++) {
    for (int tileY = maxYTile; tileY <= minYTile; tileY++) {
      std::cout << std::string("") + std::to_string(zoom) + "/" +
                       std::to_string(tileY) + "/" + std::to_string(tileX)
                << std::endl;
      std::string output =
          ws.get(std::string("") + std::to_string(zoom) + "/" +
                 std::to_string(tileY) + "/" + std::to_string(tileX));
      Image img((unsigned char*)(output.c_str()), output.size());
      // img.saveAs(std::string("tile-") + std::to_string(zoom) + "-" +
      // std::to_string(tileY) + "-" + std::to_string(tileX) + ".png");

      int xStart = (tileX - minXTile) * 256;
      int yStart = (tileY - maxYTile) * 256;

      for (int x = 0; x < 256; x++) {
        for (int y = 0; y < 256; y++) {
          combined.setPixel(xStart + x, yStart + y, img.getPixel(x, y));
        }
      }
    }
  }
  // combined.saveAs("combined.png");

  float imageMinLon = tileToLongitude(minXTile, zoom);
  float imageMaxLon = tileToLongitude(maxXTile + 1, zoom);
  float imageMinLat = tileToLatitude(minYTile + 1, zoom);
  float imageMaxLat = tileToLatitude(maxYTile, zoom);
  int newXStart =
      width * ((minLon - imageMinLon) / (imageMaxLon - imageMinLon));
  int newXEnd = width * ((maxLon - imageMinLon) / (imageMaxLon - imageMinLon));
  int newYStart =
      height * (1 - (maxLat - imageMinLat) / (imageMaxLat - imageMinLat));
  int newYEnd =
      height * (1 - (minLat - imageMinLat) / (imageMaxLat - imageMinLat));
  std::cout << imageMinLat << " " << imageMaxLat << " " << newYStart << " "
            << newYEnd << std::endl;
  std::cout << imageMinLon << " " << imageMaxLon << " " << newXStart << " "
            << newXEnd << std::endl;

  int finalWidth = newXEnd - newXStart;
  int finalHeight = newYEnd - newYStart;

  Image* final = new Image(finalWidth, finalHeight);
  for (int x = 0; x < finalWidth; x++) {
    for (int y = 0; y < finalHeight; y++) {
      final->setPixel(x, y, combined.getPixel(x + newXStart, y + newYStart));
    }
  }

  return final;
}

Image* TerrainAPI::CreateElevationImage(float minLon, float minLat,
                                        float maxLon, float maxLat) {
  std::cout << "Elevation image has not been implemented." << std::endl;
  return nullptr;
}

}  // namespace gis
