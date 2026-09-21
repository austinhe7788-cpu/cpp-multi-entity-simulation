#include "sim/entities/Predator.h"

#include "sim/entities/BraitenbergVehicle.h"
#include "sim/entities/Light.h"

Predator::Predator(Vector3 pos, Vector3 dir, double radius, double arenaWidth,
                   double arenaHeight, const json& motion, double capacity,
                   const json& disguise)
    : Robot(pos, dir, radius, arenaWidth, arenaHeight, motion, capacity),
      disguiseType_1("predator"),
      revealed_1(true) {
  SetType<Predator>("predator");

  if (disguise.is_string()) {
    disguiseType_1 = disguise.get<std::string>();
    if (disguiseType_1 == "bv") {
      disguiseType_1 = "braitenbergvehicle";
    }
    revealed_1 = false;
    if (disguiseType_1 == "light") {
      SetType<Light>("light");
    } else if (disguiseType_1 == "robot") {
      SetType<Robot>("robot");
    } else if (disguiseType_1 == "braitenbergvehicle") {
      SetType<BraitenbergVehicle>("braitenbergvehicle");
    } else {
      SetType<Predator>("predator");
    }

  } else if (disguise.is_object() && disguise.contains("type")) {
    disguiseType_1 = disguise["type"].get<std::string>();
    if (disguiseType_1 == "bv") {
      disguiseType_1 = "braitenbergvehicle";
    }
    revealed_1 = false;

    if (disguiseType_1 == "light") {
      SetType<Light>("light");
    } else if (disguiseType_1 == "robot") {
      SetType<Robot>("robot");
    } else if (disguiseType_1 == "braitenbergvehicle") {
      SetType<BraitenbergVehicle>("braitenbergvehicle");
    } else {
      SetType<Predator>("predator");
    }
  }
}
void Predator::Update(double dt) { Robot::Update(dt); }

const std::string& Predator::GetVisibleType() const {
  if (revealed_1) {
    static const std::string predatorType = "predator";
    return predatorType;
  }

  if (disguiseType_1 == "bv") {
    static const std::string bvType = "braitenbergvehicle";
    return bvType;
  }

  return disguiseType_1;
}

const std::string& Predator::GetTypeName() const { return GetVisibleType(); }

void Predator::Reveal() {
  revealed_1 = true;
  SetType<Predator>("predator");
}

double Predator::Attack(Robot& target) {
  Reveal();

  double drainedAmount_1 = target.GetCharge();
  if (drainedAmount_1 <= 0.0) {
    return 0.0;
  }

  target.SetCharge(0.0);
  target.SetDirection(Vector3(0.0, 0.0, 0.0));
  target.SetSpeed(0.0);

  Recharge(drainedAmount_1);
  return drainedAmount_1;
}
