#ifndef COMPOSITE_MOTION_H_
#define COMPOSITE_MOTION_H_

#include <memory>
#include <vector>

#include "sim/motions/IMotion.h"

class CompositeMotion : public IMotion {
 public:
  CompositeMotion() = default;
  virtual ~CompositeMotion() = default;

  void AddChild(std::unique_ptr<IMotion> child);

  void Update(EntityBase& entity, double dt) override;

 protected:
  std::vector<std::unique_ptr<IMotion>> children_;
};

#endif