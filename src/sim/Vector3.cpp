/**
 * @file Vector3.cpp
 *
 * @copyright 2026 3081 Staff, All rights reserved.
 */

#include "sim/Vector3.h"

Vector3::Vector3() {
  vec[0] = 0.0;
  vec[1] = 0.0;
  vec[2] = 0.0;
}

Vector3::Vector3(double x, double y, double z) {
  vec[0] = x;
  vec[1] = y;
  vec[2] = z;
}

Vector3 Vector3::operator*(double val) {
  return Vector3(vec[0] * val, vec[1] * val, vec[2] * val);
}

Vector3 Vector3::operator/(double val) {
  return Vector3(vec[0] / val, vec[1] / val, vec[2] / val);
}

Vector3 Vector3::operator+(const Vector3 v) {
  return Vector3(vec[0] + v[0], vec[1] + v[1], vec[2] + v[2]);
}

Vector3 Vector3::operator-(const Vector3 v) {
  return Vector3(vec[0] - v[0], vec[1] - v[1], vec[2] - v[2]);
}

double Vector3::Length() {
  return std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

Vector3 Vector3::Normalized() {
  double l = Length();
  if (l < 0.0001) {
    return Vector3(1.0, 0.0, 0.0);
  }
  return (*this) / l;
}
