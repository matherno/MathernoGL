#pragma once

#include <boost/random.hpp>

#include <mathernogl/maths/RandomGenerator.h>
#include <mathernogl/maths/Vector2D.h>
#include <mathernogl/maths/Vector3D.h>
#include <mathernogl/maths/Vector4D.h>

namespace mathernogl {

/*
 *	creates a random noise function with given frequency
 *	uses a float parameter and a random seed generated in constructor to generate a random value between the min and max bounds
 *	the same parameter will return the same value each time for a given noise function
 *	uses cosine interpolation
 */
class NoiseGenerator {
private:
	float minNoise = 0.0;
	float maxNoise = 1.0;
	float waveLength = 1.0;
	unsigned int seed = 0;

public:
	NoiseGenerator();
	NoiseGenerator(unsigned int seed);
	void setBounds(float min, float max);
	void setWaveLength(float waveLength);
	float getNoise(float noiseParameter) const;
	float getSmoothedNoise(float noiseParameter) const;

private:
	float getNoiseAtIntegerPoint(int noiseParameter) const;
	float getNoiseAtIntegerPointSmoothed(int noiseParameter) const;
};

}
