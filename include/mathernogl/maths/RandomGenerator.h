#pragma once

#include <random>

#include <mathernogl/maths/Vector3D.h>

namespace mathernogl {

class RandomGenerator {
public:
  static float randomFloat(float min, float max);
  static float randomFloat();
  static double randomDouble(double min, double max);
  static double randomDouble();
  static int randomInt(int min, int max);
  static void setSeed(uint seed = static_cast <unsigned> (time(0)));

private:
	std::random_device generator;
	double boundMin = 0, boundMax = 1;

public:
	RandomGenerator();
	void setBounds(double min, double max);
	int getIntValue();
	float getFloatValue();
	double getDoubleValue();
	int getIntValue(int min, int max);
	float getFloatValue(float min, float max);
	double getDoubleValue(double min, double max);
	Vector3D getNormal();

  //random point within a sphere
  Vector3D getPointInSphere(float radius = 1);
	bool getBoolean(float probability = 0.5f);
};

}
