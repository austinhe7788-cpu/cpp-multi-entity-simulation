#include "sim/motions/AroundMotion.h"

#include <cmath>

#include "sim/entities/EntityBase.h"

AroundMotion::AroundMotion(Vector3 center, double radius, double speed)
    : center_(center),
      radius_(radius),
      speed_(speed),
      angle_(0.0),
      initialized_(false) {}

void AroundMotion::Update(EntityBase& entity, double dt) {
  if (!initialized_) {
    Vector3 start = entity.GetPositionVector() - center_;
    angle_ = std::atan2(start[1], start[0]);
    initialized_ = true;
  }

  if (radius_ > 0.0) {
    double angularVelocity = std::abs(speed_) / radius_;

    if (speed_ > 0) {
      angle_ -= angularVelocity * dt;
    } else {
      angle_ += angularVelocity * dt;
    }
  }

  double x = center_[0] + radius_ * std::cos(angle_);
  double y = center_[1] + radius_ * std::sin(angle_);

  Vector3 oldPos = entity.GetPositionVector();

  Vector3 newPos(x, y, 0);
  Vector3 dir = (newPos - oldPos).Normalized();

  entity.SetPosition(newPos);
  entity.SetDirection(dir);
  entity.SetSpeed(speed_);
}
