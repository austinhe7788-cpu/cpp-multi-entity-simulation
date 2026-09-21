/**
 * @file Vector3.h
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#ifndef VECTOR3_H_
#define VECTOR3_H_

#include <cmath>

/**
 * @brief The Vector3 class represents a 3D Vector as a mathematical object.
 */
class Vector3 {
 public:
  /**
   * @brief Default constructor initializes vector to (0,0,0)
   */
  Vector3();

  /**
   * @brief Constructor initializes vector to (x,y,z)
   */
  Vector3(double x, double y, double z);

  /**
   * @brief Access operator to modify vector components
   */
  double operator[](int index) const { return vec[index]; }

  /**
   * @brief Access operator to modify vector components
   */
  double& operator[](int index) { return vec[index]; }

  /**
   * @brief Override * operator
   * @param val Scalar value to multiply with
   * @return New Vector3 that is the result of the multiplication
   */
  Vector3 operator*(double val);

  /**
   * @brief Override / operator
   * @param val Scalar value to divide by
   * @return New Vector3 that is the result of the division
   */
  Vector3 operator/(double val);

  /**
   * @brief Override + operator
   * @param v Vector3 to add
   * @return New Vector3 that is the result of the addition
   */
  Vector3 operator+(const Vector3 v);

  /**
   * @brief Override - operator
   * @param v Vector3 to subtract
   * @return New Vector3 that is the result of the subtraction
   */
  Vector3 operator-(const Vector3 v);

  /**
   * @brief Represents an abstract simulation model that contains entities in a
   * scene.
   */
  double Length();

  /**
   * @brief Returns a normalized version of the vector
   * @return New Vector3 that is the normalized version
   */
  Vector3 Normalized();

  /**
   * @brief Returns the vector as a double array.
   * @return A double array.
   */
  const double* GetArray() const { return vec; }

 private:
  double vec[3];
};

#endif