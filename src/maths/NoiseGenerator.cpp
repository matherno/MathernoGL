#include <maths/NoiseGenerator.h>

namespace mathernogl {

NoiseGenerator::NoiseGenerator() {
	RandomGenerator random;
	seed = random.getIntValue(0, INT32_MAX);
}

NoiseGenerator::NoiseGenerator(unsigned int seed) {
	this->seed = seed;
}

void NoiseGenerator::setBounds(float min, float max) {
	minNoise = min;
	maxNoise = max;
}

void NoiseGenerator::setWaveLength(float waveLength) {
	this->waveLength = waveLength;
}

//obtains a noise value using the given parameter
//interpolates between integer points in the function
float NoiseGenerator::getNoise(float noiseParameter) const {
	if(waveLength != 0.0f){
		noiseParameter /= waveLength;
	}
	else{
		noiseParameter = 0.0f;
	}

	const int noiseParameterFloored = floorf(noiseParameter);
	const float interpolation = noiseParameter - noiseParameterFloored;
	const float noiseValue1 = getNoiseAtIntegerPoint(noiseParameterFloored);
	const float noiseValue2 = getNoiseAtIntegerPoint(noiseParameterFloored + 1);
	const float cosineMix = (float)(1.0 - cos(interpolation * M_PI)) * 0.5;
	return noiseValue1 * (1.0 - cosineMix) + noiseValue2 * cosineMix;
}

//obtains a noise value using the given parameter
//interpolates between integer points in the function
//function is smoother than by using getNoise()
float NoiseGenerator::getSmoothedNoise(float noiseParameter) const {
	if(waveLength != 0.0f){
		noiseParameter /= waveLength;
	}
	else{
		noiseParameter = 0.0f;
	}

	const int noiseParameterFloored = floorf(noiseParameter);
	const float interpolation = noiseParameter - noiseParameterFloored;
	const float noiseValue1 = getNoiseAtIntegerPointSmoothed(noiseParameterFloored);
	const float noiseValue2 = getNoiseAtIntegerPointSmoothed(noiseParameterFloored + 1);
	const float cosineMix = (float)(1.0 - cos(interpolation * M_PI)) * 0.5;
	return noiseValue1 * (1.0 - cosineMix) + noiseValue2 * cosineMix;
}

//obtains the random value obtained when using an adjusted seed using the noiseParameter
float NoiseGenerator::getNoiseAtIntegerPoint(int noiseParameter) const {
	const long adjustedSeed = seed + noiseParameter*23;
	boost::mt19937 generator(adjustedSeed);
	boost::uniform_real<> distribution(minNoise, maxNoise);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<>> random(generator, distribution);
	return random();
}

//obtains the random value obtained when using an adjusted seed using the noiseParameter
//smoothes the value obtained by averaging it with its neighbours
float NoiseGenerator::getNoiseAtIntegerPointSmoothed(int noiseParameter) const {
	float smoothedNoise = 0.0;
	smoothedNoise += (getNoiseAtIntegerPoint(noiseParameter - 1) / 4);
	smoothedNoise += getNoiseAtIntegerPoint(noiseParameter);
	smoothedNoise += (getNoiseAtIntegerPoint(noiseParameter + 1) / 4);
	return smoothedNoise / 3.0;
}

}
