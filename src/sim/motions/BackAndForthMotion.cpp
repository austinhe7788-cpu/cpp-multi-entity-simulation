#include "sim/motions/BackAndForthMotion.h"

#include "sim/entities/EntityBase.h"

BackAndForthMotion::BackAndForthMotion(const std::vector<Vector3>& points,
                                       double speed)
    : points_(points), currentIndex_(0), speed_(speed) {}

void BackAndForthMotion::Update(EntityBase& entity, double dt) {
  if (points_.size() < 2) return;

  Vector3 pos = entity.GetPositionVector();
  Vector3 target = points_[currentIndex_];

  Vector3 toTarget = target - pos;
  double dist = toTarget.Length();

  if (dist <= speed_ * dt) {
    entity.SetPosition(target);
    currentIndex_ = (currentIndex_ + 1) % points_.size();

    Vector3 nextTarget = points_[currentIndex_];
    Vector3 newDir = (nextTarget - target).Normalized();
    entity.SetDirection(newDir);
  } else {
    Vector3 dir = toTarget.Normalized();
    Vector3 newPos = pos + dir * speed_ * dt;

    entity.SetPosition(newPos);
    entity.SetDirection(dir);
  }

  entity.SetSpeed(speed_);
}