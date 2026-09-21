#ifndef BACK_AND_FORTH_MOTION_H_
#define BACK_AND_FORTH_MOTION_H_

#include <vector>

#include "sim/Vector3.h"
#include "sim/motions/IMotion.h"

class BackAndForthMotion : public IMotion {
 public:
  BackAndForthMotion(const std::vector<Vector3>& points, double speed);

  void Update(EntityBase& entity, double dt) override;

 private:
  std::vector<Vector3> points_;
  int currentIndex_;
  double speed_;
};

#endif