#include <mathernogl/maths/RandomGenerator.h>
#include <mathernogl/maths/Maths.h>

namespace mathernogl {


void RandomGenerator::setSeed(uint seed) {
  srand(seed);
}

float RandomGenerator::randomFloat(float min, float max) {
  return min + ((float)rand() / RAND_MAX) * (max-min);
}

float RandomGenerator::randomFloat() {
  return (float)rand() / RAND_MAX;
}

int RandomGenerator::randomInt(int min, int max) {
  return min + (rand() % (max-min+1));
}

//basically skipping the first generated value, as it tends to be an strange non-random number
RandomGenerator::RandomGenerator() {
	getFloatValue();
	getIntValue();
}

void RandomGenerator::setBounds(float min, float max) {
	if (min >= max) {
		min = max - 1;
	}

	boundMin = min;
	boundMax = max;
}

int RandomGenerator::getIntValue() {
	return getIntValue((int)boundMin, (int)boundMax);
}

float RandomGenerator::getFloatValue() {
	return getFloatValue(boundMin, boundMax);
}

int RandomGenerator::getIntValue(int min, int max) {
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(generator);
}

float RandomGenerator::getFloatValue(float min, float max) {
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(generator);
}

bool RandomGenerator::getBoolean(float probability /*= 0.5f*/) {
	if (getFloatValue(0, 1) < probability) {
		return true;
	}
}

Vector3D RandomGenerator::getNormal() {
	return getPointInSphere(1).getUniform();
}

Vector3D RandomGenerator::getPointInSphere(float radius/*= 1*/) {
  mathernogl::Vector3D point(0);
  if(radius > 0)
  {
    do{
      point.x = getFloatValue(-radius, radius);
      point.y = getFloatValue(-radius, radius);
      point.z = getFloatValue(-radius, radius);
    } while (point.magnitude() >= radius);
  }
  return point;
}



}
