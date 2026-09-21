#ifndef WATER_H_
#define WATER_H_

#include <string>

#include "sim/entities/EntityBase.h"

// Water entity
class Water : public EntityBase {
 public:
  // constructor
  Water(Vector3 pos, Vector3 dir, double radius, double arenaWidth,
        double arenaHeight, const json& motion);

  void Update(double dt) override;

  const std::string& GetMotionType() const { return motionType_1; }

 protected:
  Vector3 vel_1;
  double arenaWidth_1;
  double arenaHeight_1;
  std::string motionType_1;
};

#endif