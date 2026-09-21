/**
 * @file Light.cpp
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#include "sim/entities/Light.h"

#include <memory>

#include "sim/motions/AroundMotion.h"
#include "sim/motions/BackAndForthMotion.h"
#include "sim/motions/BounceMotion.h"
#include "sim/motions/BraitenbergBehaviorMotion.h"
#include "sim/motions/CompositeMotion.h"
#include "sim/motions/DefaultMotion.h"
#include "sim/motions/InverseDistanceMotion.h"
#include "sim/motions/KeyboardMotion.h"

Light::Light(Vector3 pos, Vector3 dir, double radius, double arenaWidth,
             double arenaHeight, const json& motion) {
  SetType<Light>("Light");

  this->pos = pos;
  this->dir = dir;
  this->radius = radius;

  arenaWidth_1 = arenaWidth;
  arenaHeight_1 = arenaHeight;

  speed = 0.0;
  if (motion.contains("speed")) {
    speed = motion["speed"];
  }

  motionType_1 = "default";
  if (motion.contains("type")) {
    motionType_1 = motion["type"];
  }

  vel_1 = dir * speed;

  if (motionType_1 == "default") {
    SetMotion(std::make_unique<DefaultMotion>());
  } else if (motionType_1 == "simple" || motionType_1 == "bounce") {
    SetMotion(
        std::make_unique<BounceMotion>(vel_1, arenaWidth_1, arenaHeight_1));
  } else if (motionType_1 == "keyboard") {
    std::vector<std::string> keys;
    if (motion.contains("keys")) {
      for (const auto& key : motion["keys"]) {
        keys.push_back(key.get<std::string>());
      }
    }

    SetMotion(std::make_unique<KeyboardMotion>(speed, arenaWidth_1,
                                               arenaHeight_1, keys));
  } else if (motionType_1 == "back_and_forth") {
    std::vector<Vector3> pts;
    for (auto& p : motion["points"]) {
      pts.push_back(Vector3(p[0], p[1], p[2]));
    }
    SetMotion(std::make_unique<BackAndForthMotion>(pts, speed));
  } else if (motionType_1 == "around") {
    Vector3 center(motion["center"][0], motion["center"][1],
                   motion["center"][2]);
    double r = motion["radius"];
    double aroundSpeed = speed;

    if (motion.contains("direction") &&
        motion["direction"] == "counterclockwise") {
      aroundSpeed = -std::abs(speed);
    } else if (motion.contains("clockwise") && motion["clockwise"] == false) {
      aroundSpeed = -std::abs(speed);
    }

    SetMotion(std::make_unique<AroundMotion>(center, r, aroundSpeed));
  } else if (motionType_1 == "inverse-weighted") {
    std::vector<std::string> entityTypes;
    if (motion.contains("entity-type")) {
      for (const auto& t : motion["entity-type"]) {
        entityTypes.push_back(t.get<std::string>());
      }
    }

    double searchRadius = 0.0;
    if (motion.contains("search-radius")) {
      searchRadius = motion["search-radius"];
    }

    auto inverseMotion =
        std::make_unique<InverseDistanceMotion>(entityTypes, searchRadius);

    if (motion.contains("children")) {
      for (const auto& child : motion["children"]) {
        std::string childType = "default";
        if (child.contains("type")) {
          childType = child["type"];
        }

        double childSpeed = speed;
        if (child.contains("speed")) {
          childSpeed = child["speed"];
        }

        if (childType == "braitenberg_behavior" || childType == "explore" ||
            childType == "love" || childType == "fear" ||
            childType == "agression" || childType == "aggression") {
          std::string behavior = childType;
          if (behavior == "braitenberg_behavior") {
            behavior = "explore";
          }
          if (behavior == "agression") {
            behavior = "aggression";
          }

          std::string target = "light";
          if (child.contains("behavior")) {
            behavior = child["behavior"];
          }
          if (child.contains("target")) {
            target = child["target"];
          }

          double maxSpeed = 80.0;
          if (child.contains("maxSpeed")) {
            maxSpeed = child["maxSpeed"];
          }

          inverseMotion->AddChild(std::make_unique<BraitenbergBehaviorMotion>(
              behavior, target, childSpeed, maxSpeed, arenaWidth_1,
              arenaHeight_1));
        }
      }
    }

    SetMotion(std::move(inverseMotion));
  } else if (motionType_1 == "braitenberg_behavior" ||
             motionType_1 == "explore" || motionType_1 == "love" ||
             motionType_1 == "fear" || motionType_1 == "agression" ||
             motionType_1 == "aggression") {
    std::string behavior = motionType_1;
    if (behavior == "braitenberg_behavior") {
      behavior = "explore";
    }
    if (behavior == "agression") {
      behavior = "aggression";
    }
    std::string target = "light";
    double maxSpeed = 80.0;
    if (motion.contains("maxSpeed")) {
      maxSpeed = motion["maxSpeed"];
    }

    if (motion.contains("behavior")) {
      behavior = motion["behavior"];
    }
    if (motion.contains("target")) {
      target = motion["target"];
    }
    if (motion.contains("maxSpeed")) {
      maxSpeed = motion["maxSpeed"];
    }

    SetMotion(std::make_unique<BraitenbergBehaviorMotion>(
        behavior, target, speed, maxSpeed, arenaWidth_1, arenaHeight_1));
  } else {
    SetMotion(std::make_unique<DefaultMotion>());
  }
}

void Light::Update(double dt) { EntityBase::Update(dt); }

double Light::GetIntensity() const {
  if (radius <= 0.0) {
    return 0.0;
  }
  return 100.0 / radius;
}
