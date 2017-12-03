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
    assert(sizeof(float) == sizeof(int));
    if (a == b)
        return true;
    int intDiff = abs(*(int*)&a - *(int*)&b);
    return maxUlps >= intDiff;
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

}
