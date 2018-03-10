//
// Created by matt on 5/06/17.
//

#include "mathernogl/maths/Arithmetic.h"
#include <cmath>
#include <assert.h>
#include <complex>

namespace mathernogl{

double radToDeg(double radians) {
    return radians * 180 / pi;
}

double degToRad(double degrees) {
    return degrees * pi / 180;
}

bool compareFloat(float a, float b, unsigned int maxUlps){
  static_assert(sizeof(float) == sizeof(int), "");
    if (a == b)
        return true;
    int intDiff = abs(*(int*)&a - *(int*)&b);
    return maxUlps >= intDiff;
}

double clampd(double value, double min, double max){
  return std::max(std::min(value, max), min);
}

float clampf(float value, float min, float max){
	return std::max(std::min(value, max), min);
}

int clampi(int value, int min, int max) {
  if(value < min)
    return min;
  if(value > max)
    return max;
  return value;
}

double ccwAngleBetween(const Vector2D& first, const Vector2D& second) {
  Vector2D f = first;
  Vector2D s = second;
  f.makeUniform();
  s.makeUniform();
  double dotProduct = f.x*s.x + f.y*s.y;
  double determinant = f.x*s.y - f.y*s.x;
  double angle = atan2(determinant, dotProduct);
  return -1.0f * (float)radToDeg(angle);
}

Vector3D lerpVector3D(const Vector3D& first, const Vector3D& second, double factor){
  factor = clampd(factor, 0, 1);
  return (first * (1.0f-factor)) + (second * factor);
}

}
