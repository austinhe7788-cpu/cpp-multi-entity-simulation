

/**
 * @file DefaultMotion.h
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#ifndef DEFAULT_MOTION_H_
#define DEFAULT_MOTION_H_

#include "sim/motions/IMotion.h"

/**
 * @brief A motion strategy that keeps an entity stationary.
 */
class DefaultMotion : public IMotion {
 public:
  /**
   * @brief Default constructor.
   */
  DefaultMotion() {}

  /**
   * @brief Virtual destructor.
   */
  virtual ~DefaultMotion() {}

  /**
   * @brief Updates an entity using default motion behavior.
   * @param entity The entity being updated.
   * @param dt The elapsed timestep.
   */
  void Update(EntityBase& entity, double dt) override;
};

#endif