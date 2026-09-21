/**
 * @file DifferentialDriveKinematics.cpp
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#include "sim/physics/DifferentialDriveKinematics.h"

#include <cmath>

DifferentialDriveKinematics::DifferentialDriveKinematics(
    double radius, const Vector3& position, const Vector3& direction) {
  this->radius = radius;
  this->pose = Pose(position[0], position[1],
                    rad2deg(std::atan2(direction[1], direction[0])));
}

void DifferentialDriveKinematics::SetDirection(const Vector3& direction) {
  this->pose = Pose(this->pose.x, this->pose.y,
                    rad2deg(std::atan2(direction[1], direction[0])));
}

void DifferentialDriveKinematics::Update(double dt, double left, double right,
                                         Vector3& pos, Vector3& velocity) {
  double x_prime, y_prime, theta_prime;

  // Get the current pose (position and heading of the composing entity)
  // struct Pose pose = entity_->get_pose();
  // Store the velocity for other functions to use during this calculation.
  float temp_vel_left = left;
  float temp_vel_right = right;

  // If there is a difference between wheel speeds, use differential drive
  // model to calculate new pose.
  if (std::fabs(temp_vel_left - temp_vel_right) > 0) { /* general case */
    struct Pose icc = calc_icc(pose, temp_vel_left, temp_vel_right);
    // This can be used for debugging to improve motion control
    /*if (0) {
    printf("radius: %f\n", Radius());
    printf("vr: %f vl: %f Omega: %f\n",
        temp_vel_right, temp_vel_left, omega(temp_vel_left, temp_vel_right));
    printf("icc radius: %f\n", icc_radius(temp_vel_left, temp_vel_right));
    printf("icc: %f %f\n", icc.x, icc.y);
    }*/
    // Foodd on differential drive model cited in the header.
    x_prime =
        (pose.x - icc.x) * std::cos(omega(temp_vel_left, temp_vel_right) * dt) +
        (pose.y - icc.y) *
            -std::sin(omega(temp_vel_left, temp_vel_right) * dt) +
        icc.x;
    y_prime =
        (pose.x - icc.x) * std::sin(omega(temp_vel_left, temp_vel_right) * dt) +
        (pose.y - icc.y) * std::cos(omega(temp_vel_left, temp_vel_right) * dt) +
        icc.y;
    theta_prime = pose.theta + omega(temp_vel_left, temp_vel_right) * dt;
  } else {
    // V_r = V_l. Drive straight in the direction of thet heading.
    x_prime = pose.x + std::cos(deg2rad(pose.theta)) * temp_vel_left * dt;
    y_prime = pose.y + std::sin(deg2rad(pose.theta)) * temp_vel_left * dt;
    theta_prime = pose.theta;
  }
  pose = Pose(x_prime, y_prime, theta_prime);

  pos[0] = pose.x;
  pos[1] = pose.y;
  pos[2] = 0.0;

  velocity[0] = std::cos(deg2rad(pose.theta));
  velocity[1] = std::sin(deg2rad(pose.theta));
  velocity[2] = 0.0;
}

struct DifferentialDriveKinematics::Pose DifferentialDriveKinematics::calc_icc(
    struct Pose pose, double temp_vel_left, double temp_vel_right) const {
  return Pose(pose.x - icc_radius(temp_vel_left, temp_vel_right) *
                           std::sin(deg2rad(pose.theta)),
              pose.y + icc_radius(temp_vel_left, temp_vel_right) *
                           std::cos(deg2rad(pose.theta)));
} /* calc_icc() */

double DifferentialDriveKinematics::icc_radius(double temp_vel_left,
                                               double temp_vel_right) const {
  /*
   * Assuming a radius of 0.5, regardless of radius of actual entity. Otherwise
   * things look weird.
   */
  return (0.5 * (temp_vel_left + temp_vel_right) /
          (temp_vel_right - temp_vel_left));
} /* icc_radius() */

double DifferentialDriveKinematics::omega(double temp_vel_left,
                                          double temp_vel_right) const {
  /*
   * Assuming a radius of 0.5, regardless of radius of actual entity. Otherwise
   * things look weird.
   */
  return (temp_vel_right - temp_vel_left) / 0.5;
}
