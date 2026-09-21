#include "sim/motions/CompositeMotion.h"

void CompositeMotion::AddChild(std::unique_ptr<IMotion> child) {
  if (child) {
    children_.push_back(std::move(child));
  }
}

void CompositeMotion::Update(EntityBase& entity, double dt) {
  for (auto& child : children_) {
    child->Update(entity, dt);
  }
}