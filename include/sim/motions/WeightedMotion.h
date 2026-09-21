#ifndef WEIGHTED_MOTION_H_
#define WEIGHTED_MOTION_H_

#include <vector>

#include "sim/motions/CompositeMotion.h"

class WeightedMotion : public CompositeMotion {
 public:
  WeightedMotion() = default;
  virtual ~WeightedMotion() = default;

  void Update(EntityBase& entity, double dt) override {
    if (children_.empty()) {
      return;
    }

    std::vector<double> weights = GetWeights(entity);
    if (weights.size() != children_.size()) {
      return;
    }

    for (int i = 0; i < children_.size(); i++) {
      double w = weights[i];
      if (w < 0.0) {
        w = 0.0;
      }
      children_[i]->Update(entity, dt * w);
    }
  }

 protected:
  virtual std::vector<double> GetWeights(EntityBase& entity) = 0;
};

#endif