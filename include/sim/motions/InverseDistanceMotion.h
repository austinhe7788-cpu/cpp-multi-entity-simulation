#ifndef INVERSE_DISTANCE_MOTION_H_
#define INVERSE_DISTANCE_MOTION_H_

#include <string>
#include <vector>

#include "core/IEntity.h"
#include "sim/motions/WeightedMotion.h"

class InverseDistanceMotion : public WeightedMotion {
 public:
  InverseDistanceMotion(const std::vector<std::string>& entityTypes,
                        double searchRadius);

  void SetEntities(const std::vector<IEntity*>* entities);

 protected:
  std::vector<double> GetWeights(EntityBase& entity) override;

 private:
  bool MatchType(IEntity* candidate, const std::string& type) const;

  std::vector<std::string> entityTypes_;
  double searchRadius_;
  const std::vector<IEntity*>* entities_;
};

#endif