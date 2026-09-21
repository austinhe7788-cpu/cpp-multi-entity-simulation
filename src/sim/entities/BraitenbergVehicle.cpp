
// BY Chia Chi Lin
#define _USE_MATH_DEFINES
#include "sim/entities/BraitenbergVehicle.h"

#include <cstdlib>
#include <ctime>

#include "sim/entities/Energy.h"
#include "sim/entities/Predator.h"
#include "sim/entities/Robot.h"
#include "sim/entities/Water.h"
#include "sim/motions/BraitenbergBehaviorMotion.h"

BraitenbergVehicle::BraitenbergVehicle(Vector3 pos, Vector3 dir, double radius,
                                       double arenaWidth, double arenaHeight,
                                       const json& motion, double capacity)
    : Robot(pos, dir, radius, arenaWidth, arenaHeight, motion, capacity),
      arenaWidth_1(arenaWidth),
      arenaHeight_1(arenaHeight),
      kinematics_(radius, pos, dir) {
  SetType<BraitenbergVehicle>("BraitenbergVehicle");

  if (motion.contains("behavior")) {
    behavior_1 = motion["behavior"].get<std::string>();
  }
  if (motion.contains("type")) {
    std::string motionType = motion["type"].get<std::string>();
    if (motionType == "love" || motionType == "explore" ||
        motionType == "fear" || motionType == "agression" ||
        motionType == "aggression") {
      behavior_1 = motionType;
    }
  }
  if (motion.contains("maxSpeed")) {
    maxSpeed_1 = motion["maxSpeed"].get<double>();
  }
  if (motion.contains("speed")) {
    speedFactor_1 = motion["speed"].get<double>();
  }
  if (motion.contains("target")) {
    targetType_1 = motion["target"].get<std::string>();
  }
  if (behavior_1 == "agression") {
    behavior_1 = "aggression";
  }

  SetMotion(std::make_unique<BraitenbergBehaviorMotion>(
      behavior_1, targetType_1, speedFactor_1, maxSpeed_1, arenaWidth_1,
      arenaHeight_1));

  vel_1 = Vector3(0, 0, 0);
  UpdateSensorPositions();
}

void BraitenbergVehicle::Update(double dt,
                                const std::vector<IEntity*>& entities) {
  BraitenbergBehaviorMotion* behaviorMotion =
      dynamic_cast<BraitenbergBehaviorMotion*>(GetMotion());

  if (behaviorMotion) {
    behaviorMotion->SetEntities(&entities);
  }

  Robot::Update(dt);
}

void BraitenbergVehicle::UpdateSensorPositions() {
  const double angle = 40.0 * M_PI / 180.0;

  Vector3 fwd = dir;
  fwd[2] = 0.0;

  if (fwd.Length() < 1e-6) {
    fwd = Vector3(1, 0, 0);
  }
  fwd = fwd.Normalized();

  // rotate fwd by +angle and -angle in XY
  Vector3 leftDir(fwd[0] * std::cos(angle) - fwd[1] * std::sin(angle),
                  fwd[0] * std::sin(angle) + fwd[1] * std::cos(angle), 0.0);

  Vector3 rightDir(fwd[0] * std::cos(-angle) - fwd[1] * std::sin(-angle),
                   fwd[0] * std::sin(-angle) + fwd[1] * std::cos(-angle), 0.0);

  leftSensorPos_1 = pos + leftDir.Normalized() * radius;
  rightSensorPos_1 = pos + rightDir.Normalized() * radius;
}

double BraitenbergVehicle::SensorReadingAt(
    const Vector3& sensorPos, const std::vector<IEntity*>& entities) const {
  IEntity* closest = nullptr;
  double bestDist = 1e18;

  for (IEntity* e : entities) {
    if (!e || e == this) {
      continue;
    }

    bool match = false;

    if (targetType_1 == "light") {
      match = dynamic_cast<Light*>(e) != nullptr &&
              dynamic_cast<Energy*>(e) == nullptr;
    } else if (targetType_1 == "energy") {
      match = dynamic_cast<Energy*>(e) != nullptr;
    } else if (targetType_1 == "water") {
      match = dynamic_cast<Water*>(e) != nullptr;
    } else if (targetType_1 == "robot") {
      match = dynamic_cast<Robot*>(e) != nullptr &&
              dynamic_cast<Predator*>(e) == nullptr &&
              dynamic_cast<BraitenbergVehicle*>(e) == nullptr;
    } else if (targetType_1 == "predator") {
      match = dynamic_cast<Predator*>(e) != nullptr;
    } else if (targetType_1 == "bv") {
      match = dynamic_cast<BraitenbergVehicle*>(e) != nullptr;
    }

    if (!match) {
      continue;
    }

    const double* p = e->GetPosition();
    Vector3 targetPos(p[0], p[1], p[2]);
    double dist = (targetPos - sensorPos).Length();

    if (dist < bestDist) {
      bestDist = dist;
      closest = e;
    }
  }

  if (!closest) {
    return 0.0001;
  }

  return std::max(1000.0 / (bestDist * bestDist + 1.0), 0.0001);
}

void BraitenbergVehicle::ClampWheels() {
  leftWheel_1 = std::max(-maxSpeed_1, std::min(maxSpeed_1, leftWheel_1));
  rightWheel_1 = std::max(-maxSpeed_1, std::min(maxSpeed_1, rightWheel_1));
}

void BraitenbergVehicle::ComputeWheelsFromReadings() {
  const double detectThreshold = 0.02;
  const double stopThreshold = 1.0;
  const double eps = 0.001;

  bool detected =
      (leftReading_1 > detectThreshold || rightReading_1 > detectThreshold);

  if (behavior_1 == "explore") {
    leftWheel_1 = speedFactor_1;
    rightWheel_1 = speedFactor_1;
    return;
  }
  if (behavior_1 == "love") {
    if (!detected) {
      leftWheel_1 = speedFactor_1;
      rightWheel_1 = speedFactor_1;
      return;
    }

    if (leftReading_1 > stopThreshold && rightReading_1 > stopThreshold) {
      leftWheel_1 = 0.0;
      rightWheel_1 = 0.0;
      return;
    }

    leftWheel_1 = speedFactor_1 / (leftReading_1 + eps);
    rightWheel_1 = speedFactor_1 / (rightReading_1 + eps);
    return;
  }
  if (behavior_1 == "fear") {
    if (!detected) {
      leftWheel_1 = 0.0;
      rightWheel_1 = 0.0;
      return;
    }

    if (leftReading_1 > stopThreshold && rightReading_1 > stopThreshold) {
      leftWheel_1 = 0.0;
      rightWheel_1 = 0.0;
      return;
    }

    leftWheel_1 = speedFactor_1 * leftReading_1;
    rightWheel_1 = speedFactor_1 * rightReading_1;
    return;
  }
  if (behavior_1 == "aggression") {
    if (!detected) {
      leftWheel_1 = 0.0;
      rightWheel_1 = 0.0;
      return;
    }

    if (leftReading_1 > stopThreshold && rightReading_1 > stopThreshold) {
      leftWheel_1 = 0.0;
      rightWheel_1 = 0.0;
      return;
    }

    leftWheel_1 = speedFactor_1 * rightReading_1;
    rightWheel_1 = speedFactor_1 * leftReading_1;
    return;
  }

  leftWheel_1 = 0.0;
  rightWheel_1 = 0.0;
}

void BraitenbergVehicle::HandleWallsAndSyncKinematics() {
  bool hit = false;

  // clamp to bounds
  if (pos[0] < radius) {
    pos[0] = radius;
    hit = true;
  }

  if (pos[0] > arenaWidth_1 - radius) {
    pos[0] = arenaWidth_1 - radius;
    hit = true;
  }

  if (pos[1] < radius) {
    pos[1] = radius;
    hit = true;
  }

  if (pos[1] > arenaHeight_1 - radius) {
    pos[1] = arenaHeight_1 - radius;
    hit = true;
  }

  if (!hit) {
    return;
  }
  // reverse direction (spec requires reverse, not random)
  dir = dir * -1.0;
  dir[2] = 0.0;
  dir = dir.Normalized();

  // keep velocity consistent with heading
  vel_1 = dir;

  // IMPORTANT: sync internal kinematics pose so it doesn't snap back next frame
  // kinematics_.ResetPose(pos, dir);
}
