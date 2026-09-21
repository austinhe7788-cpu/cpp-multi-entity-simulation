

/**
 * @file BounceMotion.h
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#ifndef BOUNCE_MOTION_H_
#define BOUNCE_MOTION_H_

#include "sim/Vector3.h"
#include "sim/motions/IMotion.h"

/**
 * @brief A motion strategy that moves an entity forward, reverses on wall
 * collision, and rotates 45 degrees two seconds after a collision.
 */
class BounceMotion : public IMotion {
 public:
  /**
   * @brief Constructs a bounce motion strategy.
   * @param velocity_1 The initial velocity vector.
   * @param arenaWidth_1 The arena width.
   * @param arenaHeight_1 The arena height.
   */
  BounceMotion(const Vector3& velocity_1, double arenaWidth_1,
               double arenaHeight_1);

  /**
   * @brief Virtual destructor.
   */
  virtual ~BounceMotion() {}

  /**
   * @brief Updates an entity using bounce motion behavior.
   * @param entity The entity being updated.
   * @param dt The elapsed timestep.
   */
  void Update(EntityBase& entity, double dt) override;

 private:
  Vector3 velocity_1;
  double arenaWidth_1;
  double arenaHeight_1;
  bool collisionMode_1;
  double collisionTime_1;
};

#endif