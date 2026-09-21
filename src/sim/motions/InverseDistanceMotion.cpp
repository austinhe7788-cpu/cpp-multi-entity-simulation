#include "sim/motions/InverseDistanceMotion.h"

#include <algorithm>
#include <cmath>

#include "sim/entities/BraitenbergVehicle.h"
#include "sim/entities/Energy.h"
#include "sim/entities/EntityBase.h"
#include "sim/entities/Light.h"
#include "sim/entities/Predator.h"
#include "sim/entities/Robot.h"
#include "sim/entities/Water.h"

InverseDistanceMotion::InverseDistanceMotion(
    const std::vector<std::string>& entityTypes, double searchRadius)
    : entityTypes_(entityTypes),
      searchRadius_(searchRadius),
      entities_(nullptr) {}

void InverseDistanceMotion::SetEntities(const std::vector<IEntity*>* entities) {
  entities_ = entities;
}

bool InverseDistanceMotion::MatchType(IEntity* candidate,
                                      const std::string& type) const {
  if (!candidate) {
    return false;
  }

  if (type == "light") {
    return dynamic_cast<Light*>(candidate) != nullptr &&
           dynamic_cast<Energy*>(candidate) == nullptr;
  }

  if (type == "energy") {
    return dynamic_cast<Energy*>(candidate) != nullptr;
  }

  if (type == "water") {
    return dynamic_cast<Water*>(candidate) != nullptr;
  }

  if (type == "robot") {
    return dynamic_cast<Robot*>(candidate) != nullptr &&
           dynamic_cast<Predator*>(candidate) == nullptr &&
           dynamic_cast<BraitenbergVehicle*>(candidate) == nullptr;
  }

  if (type == "predator") {
    return dynamic_cast<Predator*>(candidate) != nullptr;
  }

  if (type == "bv" || type == "braitenbergvehicle") {
    return dynamic_cast<BraitenbergVehicle*>(candidate) != nullptr;
  }

  return false;
}

std::vector<double> InverseDistanceMotion::GetWeights(EntityBase& entity) {
  std::vector<double> weights(children_.size(), 0.0);

  if (!entities_ || entityTypes_.size() != children_.size()) {
    return weights;
  }

  Vector3 selfPos = entity.GetPositionVector();
  double weightSum = 0.0;

  for (int i = 0; i < entityTypes_.size(); i++) {
    double bestDist = 1e18;

    for (IEntity* candidate : *entities_) {
      if (!candidate || candidate == &entity) {
        continue;
      }

      if (!MatchType(candidate, entityTypes_[i])) {
        continue;
      }

      const double* p = candidate->GetPosition();
      Vector3 otherPos(p[0], p[1], p[2]);
      double dist = (otherPos - selfPos).Length();

      if (searchRadius_ > 0.0 && dist > searchRadius_) {
        continue;
      }

      if (dist < bestDist) {
        bestDist = dist;
      }
    }

    if (bestDist < 1e18) {
      weights[i] = 1.0 / std::max(bestDist, 0.0001);
      weightSum += weights[i];
    }
  }

  if (weightSum <= 0.0) {
    return weights;
  }

  for (int i = 0; i < weights.size(); i++) {
    weights[i] /= weightSum;
  }

  return weights;
}