

/**
 * @file DefaultMotion.cpp
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#include "sim/motions/DefaultMotion.h"

#include "sim/entities/EntityBase.h"

void DefaultMotion::Update(EntityBase& entity, double dt) {
  (void)entity;
  (void)dt;
  // Default motion does nothing.
}