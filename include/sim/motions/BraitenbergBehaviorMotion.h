#ifndef BRAITENBERG_BEHAVIOR_MOTION_H_
#define BRAITENBERG_BEHAVIOR_MOTION_H_

#include <string>
#include <vector>

#include "core/IEntity.h"
#include "sim/Vector3.h"
#include "sim/motions/IMotion.h"

class BraitenbergBehaviorMotion : public IMotion {
 public:
  BraitenbergBehaviorMotion(const std::string& behavior,
                            const std::string& targetType, double speed,
                            double maxSpeed, double arenaWidth,
                            double arenaHeight);

  void Update(EntityBase& entity, double dt) override;

  void SetEntities(const std::vector<IEntity*>* entities);

 private:
  bool MatchTarget(IEntity* candidate, const EntityBase& self) const;
  IEntity* FindClosestTarget(const EntityBase& self, double& distOut) const;
  void Move(EntityBase& entity, const Vector3& direction, double dt,
            double speed) const;
  void Stop(EntityBase& entity) const;

  const std::vector<IEntity*>* entities_;

  std::string behavior_;
  std::string targetType_;

  double speed_;
  double maxSpeed_;
  double arenaWidth_;
  double arenaHeight_;
};

#endif