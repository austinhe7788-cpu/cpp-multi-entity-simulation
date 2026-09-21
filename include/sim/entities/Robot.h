/**
 * @file Robot.h
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <string>

#include "sim/entities/EntityBase.h"

/**
 * @brief Represents a robot entity in the simulation.
 *
 * A robot is an entity that can move using a motion strategy and has a limited
 * amount of energy. Robots consume energy over time and may recharge at energy
 * entities.
 */
class Robot : public EntityBase {
 public:
  /**
   * @brief Constructs a robot with the given properties and motion data.
   * @param pos The initial position of the robot.
   * @param dir The initial direction of the robot.
   * @param radius The radius of the robot.
   * @param arenaWidth The width of the arena.
   * @param arenaHeight The height of the arena.
   * @param motion The JSON object describing the motion configuration.
   * @param capacity The maximum energy capacity of the robot.
   */
  Robot(Vector3 pos, Vector3 dir, double radius, double arenaWidth,
        double arenaHeight, const json& motion, double capacity);

  /**
   * @brief Updates the robot over time.
   * @param dt A time delta since the last call to Update.
   */
  void Update(double dt) override;

  /**
   * @brief Gets the robot's configured motion type string.
   * @return A constant reference to the motion type string.
   */
  const std::string& GetMotionType() const { return motionType_1; }

  /**
   * @brief Gets the robot's maximum energy capacity.
   * @return The maximum capacity value.
   */
  double GetCapacity() const { return capacity_1; }

  /**
   * @brief Gets the robot's current charge.
   * @return The current charge value.
   */
  double GetCharge() const { return charge_1; }

  /**
   * @brief Sets the robot's current charge.
   * @param charge_1 The new charge value.
   *
   * This function should clamp the charge between 0 and capacity.
   */
  void SetCharge(double charge_1);

  /**
   * @brief Recharges the robot by a given amount.
   * @param amount_1 The amount of charge to add.
   */
  void Recharge(double amount_1);

  /**
   * @brief Drains the robot's charge by a given amount.
   * @param amount_1 The amount of charge to remove.
   */
  void DrainCharge(double amount_1);

 protected:
  /**
   * @brief Updates the robot's charge based on movement over a timestep.
   * @param dt The elapsed timestep.
   */
  void UpdateEnergy(double dt);

  /**
   * @brief Initial velocity used to configure motion strategies.
   */
  Vector3 vel_1;

  /**
   * @brief Width of the arena.
   */
  double arenaWidth_1;

  /**
   * @brief Height of the arena.
   */
  double arenaHeight_1;

  /**
   * @brief Motion type string parsed from the scene JSON.
   */
  std::string motionType_1;

  /**
   * @brief Maximum energy capacity of the robot.
   */
  double capacity_1;

  /**
   * @brief Current charge level of the robot.
   */
  double charge_1;

  /**
   * @brief Stores the robot position from the previous update step.
   *
   * This is used to estimate movement distance for energy consumption.
   */
  Vector3 previousPos_1;
};

#endif