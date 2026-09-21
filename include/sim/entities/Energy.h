
/**
 * @file Energy.h
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#ifndef ENERGY_H_
#define ENERGY_H_

#include "sim/entities/Light.h"

/**
 * @brief Represents an energy entity in the simulation.
 *
 * Energy is a special kind of Light that can recharge robots when they
 * intersect with it.
 */
class Energy : public Light {
 public:
  /**
   * @brief Constructs an Energy entity.
   * @param pos The initial position of the energy entity.
   * @param dir The initial direction of the energy entity.
   * @param radius The radius of the energy entity.
   * @param arenaWidth The width of the simulation arena.
   * @param arenaHeight The height of the simulation arena.
   * @param motion The JSON object describing the motion configuration.
   */
  Energy(Vector3 pos, Vector3 dir, double radius, double arenaWidth,
         double arenaHeight, const json& motion);

  /**
   * @brief Updates the energy entity over time.
   * @param dt A time delta since the last call to Update.
   */
  void Update(double dt) override;

  /**
   * @brief Checks whether a sensor can identify this entity as Energy.
   * @param distance_1 The distance from the sensor to this energy entity.
   * @return True if the sensor is within 5 times the radius, otherwise false.
   */
  bool IsDetectableAsEnergy(double distance_1) const;

  /**
   * @brief Calculates how much charge this energy entity can provide over time.
   * @param time_1 The recharge time interval.
   * @return The recharge amount based on time and light intensity.
   */
  double GetRechargeAmount(double time_1) const;
};

#endif