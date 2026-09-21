#ifndef AROUND_MOTION_H_
#define AROUND_MOTION_H_

#include "sim/Vector3.h"
#include "sim/motions/IMotion.h"

class AroundMotion : public IMotion {
 public:
  AroundMotion(Vector3 center, double radius, double speed);

  void Update(EntityBase& entity, double dt) override;

 private:
  Vector3 center_;
  double radius_;
  double speed_;
  double angle_;
  bool initialized_;
};

#endif
