#pragma once

#include <random>

#include <mathernogl/maths/Vector3D.h>

namespace mathernogl {

class RandomGenerator {
public:
  static float randomFloat(float min, float max);
  static float randomFloat();
  static int randomInt(int min, int max);
  static void setSeed(uint seed = static_cast <unsigned> (time(0)));

private:
	std::random_device generator;
	float boundMin = 0, boundMax = 1;

public:
	RandomGenerator();
	void setBounds(float min, float max);
	int getIntValue();
	float getFloatValue();
	int getIntValue(int min, int max);
	float getFloatValue(float min, float max);
	Vector3D getNormal();

  //random point within a sphere
  Vector3D getPointInSphere(float radius = 1);
	bool getBoolean(float probability = 0.5f);
};

}
