

/**
 * @file EntityBase.cpp
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#include "sim/entities/EntityBase.h"

EntityBase::EntityBase() {
  static int count = 0;
  id = count;
  count++;
  SetType<EntityBase>("Entity");
  radius = 10.0;
  speed = 0.0;
}

void EntityBase::Update(double dt) {
  if (motion_1) {
    motion_1->Update(*this, dt);
  } else {
    (void)dt;
  }
}

void EntityBase::SetMotion(std::unique_ptr<IMotion> motion) {
  motion_1 = std::move(motion);
}

IMotion* EntityBase::GetMotion() const { return motion_1.get(); }