

// Chia Chi

#ifndef BRAITENBERG_VEHICLE_H_
#define BRAITENBERG_VEHICLE_H_

#define _USE_MATH_DEFINES
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

#include "Light.h"
#include "sim/entities/Robot.h"
#include "sim/physics/DifferentialDriveKinematics.h"

/**
 * @brief Represents a Braitenberg vehicle entity in the simulation.
 *
 * A Braitenberg vehicle senses nearby lights and adjusts its wheel speeds based
 * on sensor readings. Its movement is modeled using differential drive
 * kinematics and constrained by the arena boundaries.
 */
class BraitenbergVehicle : public Robot {
 public:
  /**
   * @brief Constructs a BraitenbergVehicle object.
   *
   * @param pos The initial position of the vehicle.
   * @param dir The initial direction of the vehicle.
   * @param radius The radius of the vehicle.
   * @param arenaWidth The width of the simulation arena.
   * @param arenaHeight The height of the simulation arena.
   * @param motion JSON configuration containing the motion behavior settings.
   */
  BraitenbergVehicle(Vector3 pos, Vector3 dir, double radius, double arenaWidth,
                     double arenaHeight, const json& motion, double capacity);
  /**
   * @brief Updates the Braitenberg vehicle using nearby light sources.
   *
   * This function updates the vehicle's sensor positions, computes light
   * readings, adjusts wheel speeds, updates motion using differential drive
   * kinematics, and handles wall collisions.
   *
   * @param dt The time step used for the update.
   * @param lights A vector of pointers to Light objects in the simulation.
   * @return void
   */
  void Update(double dt, const std::vector<IEntity*>& entities);

  /**
   * @brief Overrides the base update function.
   *
   * This version is intentionally unused because the simulation model updates
   * the Braitenberg vehicle using the overloaded Update function that also
   * takes light sources.
   *
   * @param dt The time step used for the update.
   * @return void
   */
  void Update(double dt) override {
    Robot::Update(dt);
  }  // use SimulationModel called Update with lights instead of this one

 private:
  double arenaWidth_1 = 0.0;
  double arenaHeight_1 = 0.0;

  DifferentialDriveKinematics kinematics_;

  // BV's heading / velocity vector (not necessarily the same as kinematics
  // velocity if we are colliding with a wall and need to clamp)
  Vector3 vel_1;

  // sensors
  Vector3 leftSensorPos_1;
  Vector3 rightSensorPos_1;

  double leftReading_1 = 0.0001;
  double rightReading_1 = 0.0001;

  // wheels
  double leftWheel_1 = 0.0;
  double rightWheel_1 = 0.0;

  // behavior & limits
  std::string behavior_1 = "explore";
  double maxSpeed_1 = 80.0;
  double speedFactor_1 = 100.0;
  std::string targetType_1 = "light";
  /**
   * @brief Updates the positions of the vehicle's sensors.
   *
   * @return void
   */
  void UpdateSensorPositions();

  /**
   * @brief Computes the light reading at a given sensor position.
   *
   * This function evaluates the intensity of light detected by a sensor based
   * on its position relative to the lights in the environment.
   *
   * @param sensorPos The position of the sensor.
   * @param lights A vector of pointers to Light objects in the simulation.
   * @return The computed light intensity reading at the sensor position.
   */
  double SensorReadingAt(const Vector3& sensorPos,
                         const std::vector<IEntity*>& entities) const;

  /**
   * @brief Computes the left and right wheel speeds from the current sensor
   * readings.
   *
   * The wheel speeds are determined according to the Braitenberg vehicle's
   * behavior and current light sensor values.
   * @return void
   */
  void ComputeWheelsFromReadings();

  /**
   * @brief Clamps the wheel speeds to valid speed limits.
   *
   * This function ensures that the wheel values do not exceed the maximum
   * allowed speed for the vehicle.
   *
   * @return void
   */
  void ClampWheels();

  /**
   * @brief Handles wall collisions and synchronizes the vehicle state.
   *
   * This function prevents the vehicle from leaving the arena boundaries and
   * keeps the kinematics state consistent with the entity's position and
   * direction.
   * @return void
   */
  void HandleWallsAndSyncKinematics();
};

#endif
