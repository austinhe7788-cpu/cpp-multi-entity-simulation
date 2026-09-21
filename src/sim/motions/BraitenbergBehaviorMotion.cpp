#include "sim/motions/BraitenbergBehaviorMotion.h"

#include <algorithm>
#include <cmath>

#include "sim/entities/BraitenbergVehicle.h"
#include "sim/entities/Energy.h"
#include "sim/entities/EntityBase.h"
#include "sim/entities/Light.h"
#include "sim/entities/Predator.h"
#include "sim/entities/Robot.h"
#include "sim/entities/Water.h"

BraitenbergBehaviorMotion::BraitenbergBehaviorMotion(
    const std::string& behavior, const std::string& targetType, double speed,
    double maxSpeed, double arenaWidth, double arenaHeight)
    : entities_(nullptr),
      behavior_(behavior),
      targetType_(targetType),
      speed_(std::abs(speed)),
      maxSpeed_(std::abs(maxSpeed)),
      arenaWidth_(arenaWidth),
      arenaHeight_(arenaHeight) {
  if (behavior_ == "agression") {
    behavior_ = "aggression";
  }
}

void BraitenbergBehaviorMotion::SetEntities(
    const std::vector<IEntity*>* entities) {
  entities_ = entities;
}
bool BraitenbergBehaviorMotion::MatchTarget(IEntity* candidate,
                                            const EntityBase& self) const {
  if (!candidate || candidate == &self) {
    return false;
  }

  if (targetType_ == "light") {
    return dynamic_cast<Light*>(candidate) != nullptr &&
           dynamic_cast<Energy*>(candidate) == nullptr;
  }

  if (targetType_ == "energy") {
    return dynamic_cast<Energy*>(candidate) != nullptr;
  }

  if (targetType_ == "water") {
    return dynamic_cast<Water*>(candidate) != nullptr;
  }

  if (targetType_ == "robot") {
    return dynamic_cast<Robot*>(candidate) != nullptr &&
           dynamic_cast<Predator*>(candidate) == nullptr &&
           dynamic_cast<BraitenbergVehicle*>(candidate) == nullptr;
  }

  if (targetType_ == "predator") {
    return dynamic_cast<Predator*>(candidate) != nullptr;
  }

  if (targetType_ == "bv" || targetType_ == "braitenbergvehicle") {
    return dynamic_cast<BraitenbergVehicle*>(candidate) != nullptr;
  }

  return false;
}
IEntity* BraitenbergBehaviorMotion::FindClosestTarget(const EntityBase& self,
                                                      double& distOut) const {
  distOut = 1e18;

  if (!entities_) {
    return nullptr;
  }

  IEntity* closest = nullptr;
  Vector3 selfPos = self.GetPositionVector();

  for (IEntity* e : *entities_) {
    if (!MatchTarget(e, self)) {
      continue;
    }

    const double* p = e->GetPosition();
    Vector3 targetPos(p[0], p[1], p[2]);
    double dist = (targetPos - selfPos).Length();

    if (dist < distOut) {
      distOut = dist;
      closest = e;
    }
  }

  return closest;
}
void BraitenbergBehaviorMotion::Move(EntityBase& entity,
                                     const Vector3& direction, double dt,
                                     double speed) const {
  Vector3 dir = direction;
  dir[2] = 0.0;

  if (dir.Length() < 1e-6) {
    dir = entity.GetDirectionVector();
    dir[2] = 0.0;
  }

  if (dir.Length() < 1e-6) {
    dir = Vector3(1.0, 0.0, 0.0);
  }

  dir = dir.Normalized();

  double actualSpeed = std::abs(speed);
  if (maxSpeed_ > 0.0) {
    actualSpeed = std::min(actualSpeed, std::abs(maxSpeed_));
  }
  if (actualSpeed <= 0.0) {
    actualSpeed = (maxSpeed_ > 0.0) ? std::abs(maxSpeed_) : 50.0;
  }
  Vector3 newPos = entity.GetPositionVector() + dir * actualSpeed * dt;
  double radius = entity.GetRadius();

  if (newPos[0] < radius) {
    newPos[0] = radius;
  } else if (newPos[0] > arenaWidth_ - radius) {
    newPos[0] = arenaWidth_ - radius;
  }

  if (newPos[1] < radius) {
    newPos[1] = radius;
  } else if (newPos[1] > arenaHeight_ - radius) {
    newPos[1] = arenaHeight_ - radius;
  }

  newPos[2] = 0.0;

  entity.SetPosition(newPos);
  entity.SetDirection(dir);
  entity.SetSpeed(actualSpeed);
}

void BraitenbergBehaviorMotion::Stop(EntityBase& entity) const {
  entity.SetSpeed(0.0);
}
void BraitenbergBehaviorMotion::Update(EntityBase& entity, double dt) {
  if (dt <= 0.0) {
    return;
  }

  Vector3 currentDir = entity.GetDirectionVector();
  currentDir[2] = 0.0;

  if (currentDir.Length() < 1e-6) {
    currentDir = Vector3(1.0, 0.0, 0.0);
  }
  currentDir = currentDir.Normalized();

  double dist = 1e18;
  IEntity* closest = FindClosestTarget(entity, dist);

  double moveSpeed = (maxSpeed_ > 0.0) ? maxSpeed_ : speed_;
  if (moveSpeed <= 0.0) {
    moveSpeed = 50.0;
  }

  const double activateDist = 150.0;

  double stopDist = entity.GetRadius();
  if (closest) {
    stopDist += closest->GetRadius();
  }

  if (behavior_ == "explore") {
    Move(entity, currentDir, dt, moveSpeed);
    return;
  }

  if (behavior_ == "love") {
    if (!closest || dist > activateDist) {
      Move(entity, currentDir, dt, moveSpeed);
      return;
    }

    if (dist <= stopDist || dist - stopDist <= moveSpeed * dt) {
      Stop(entity);
      return;
    }

    const double* p = closest->GetPosition();
    Vector3 targetPos(p[0], p[1], p[2]);
    Move(entity, (targetPos - entity.GetPositionVector()).Normalized(), dt,
         moveSpeed);
    return;
  }

  if (behavior_ == "fear") {
    if (!closest || dist > activateDist) {
      Stop(entity);
      return;
    }

    if (dist <= stopDist) {
      Stop(entity);
      return;
    }

    const double* p = closest->GetPosition();
    Vector3 targetPos(p[0], p[1], p[2]);
    Move(entity, (entity.GetPositionVector() - targetPos).Normalized(), dt,
         moveSpeed);
    return;
  }

  if (behavior_ == "aggression") {
    if (!closest || dist > activateDist) {
      Stop(entity);
      return;
    }

    const double* p = closest->GetPosition();
    Vector3 targetPos(p[0], p[1], p[2]);
    Move(entity, (targetPos - entity.GetPositionVector()).Normalized(), dt,
         moveSpeed);
    return;
  }

  Stop(entity);
}