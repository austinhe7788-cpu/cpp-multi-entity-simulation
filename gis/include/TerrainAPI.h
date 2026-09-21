/**
 * @file IMap.h
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#ifndef IMAP_H_
#define IMAP_H_

#include <string>
#include <vector>

#include "Image.h"

namespace gis {

class TerrainAPI {
 public:
  virtual ~TerrainAPI() {}

  virtual Image* CreateSatelliteImage(float minLon, float minLat, float maxLon,
                                      float maxLat);
  virtual Image* CreateElevationImage(float minLon, float minLat, float maxLon,
                                      float maxLat);
};

}  // namespace gis

#endif