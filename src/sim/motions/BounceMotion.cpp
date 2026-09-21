

/**
 * @file BounceMotion.cpp
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#include "sim/motions/BounceMotion.h"

#include <cmath>

#include "sim/entities/EntityBase.h"

BounceMotion::BounceMotion(const Vector3& velocity_1, double arenaWidth_1,
                           double arenaHeight_1)
    : velocity_1(velocity_1),
      arenaWidth_1(arenaWidth_1),
      arenaHeight_1(arenaHeight_1),
      collisionMode_1(false),
      collisionTime_1(0.0) {}

void BounceMotion::Update(EntityBase& entity, double dt) {
  collisionTime_1 += dt;

  Vector3 position_1 = entity.GetPositionVector();
  double radius_1 = entity.GetRadius();

  // Euler integration
  position_1 = position_1 + velocity_1 * dt;

  // Check wall collision
  if (position_1[0] > arenaWidth_1 - radius_1 ||
      position_1[1] > arenaHeight_1 - radius_1 || position_1[0] < radius_1 ||
      position_1[1] < radius_1) {
    collisionMode_1 = true;
    collisionTime_1 = 0.0;

    // Reverse direction and move back one time step
    velocity_1 = velocity_1 * -1.0;
    position_1 = position_1 + velocity_1 * dt;
  }

  // Rotate 45 degrees two seconds after a collision
  if (collisionMode_1 && collisionTime_1 > 2.0) {
    collisionMode_1 = false;

    double angle_1 = M_PI / 4.0;
    double vx_1 =
        velocity_1[0] * std::cos(angle_1) - velocity_1[1] * std::sin(angle_1);
    double vy_1 =
        velocity_1[0] * std::sin(angle_1) + velocity_1[1] * std::cos(angle_1);

    velocity_1[0] = vx_1;
    velocity_1[1] = vy_1;

    double speed_1 = velocity_1.Length();
    velocity_1 = velocity_1.Normalized() * speed_1;
  }

  entity.SetPosition(position_1);
  entity.SetDirection(velocity_1.Normalized());
  entity.SetSpeed(velocity_1.Length());
}