

/**
 * @file Energy.cpp
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#include "sim/entities/Energy.h"

Energy::Energy(Vector3 pos, Vector3 dir, double radius, double arenaWidth,
               double arenaHeight, const json& motion)
    : Light(pos, dir, radius, arenaWidth, arenaHeight, motion) {
  SetType<Energy>("Energy");
}

void Energy::Update(double dt) { Light::Update(dt); }

bool Energy::IsDetectableAsEnergy(double distance_1) const {
  return distance_1 <= 5.0 * radius;
}

double Energy::GetRechargeAmount(double time_1) const {
  if (time_1 <= 0.0) {
    return 0.0;
  }
  return time_1 * GetIntensity() / 5.0;
}