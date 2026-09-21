/**
 * @file Light.h
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include <string>

#include "sim/entities/EntityBase.h"

/**
 * @brief Represents a light entity in the simulation.
 *
 * A Light is an entity that emits light intensity based on its radius and can
 * move according to an assigned motion strategy.
 */
class Light : public EntityBase {
 public:
  /**
   * @brief Constructs a Light object.
   *
   * @param pos The initial position of the light.
   * @param dir The initial direction of the light.
   * @param radius The radius of the light.
   * @param arenaWidth The width of the simulation arena.
   * @param arenaHeight The height of the simulation arena.
   * @param motion JSON configuration containing the light's motion behavior.
   */
  Light(Vector3 pos, Vector3 dir, double radius, double arenaWidth,
        double arenaHeight, const json& motion);

  /**
   * @brief Updates the light based on the elapsed timestep.
   * @param dt The timestep since the last update.
   */
  void Update(double dt) override;

  /**
   * @brief Gets the light intensity.
   * @return The light intensity computed from the radius.
   */
  double GetIntensity() const;

  /**
   * @brief Gets the configured motion type string.
   * @return A constant reference to the motion type string.
   */
  const std::string& GetMotionType() const { return motionType_1; }

 protected:
  /**
   * @brief Initial velocity used to configure motion strategies.
   */
  Vector3 vel_1;

  /**
   * @brief Width of the simulation arena.
   */
  double arenaWidth_1;

  /**
   * @brief Height of the simulation arena.
   */
  double arenaHeight_1;

  /**
   * @brief Motion type parsed from the JSON configuration.
   */
  std::string motionType_1;
};

#endif