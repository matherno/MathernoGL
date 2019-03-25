#pragma once


namespace mathernogl {
typedef unsigned char byte;

class ImageData {
public:
	const byte* const byteMap;
	const unsigned int width;
	const unsigned int height;
	const unsigned int bytesPerPixel;

	ImageData(byte* byteMap, unsigned int width, unsigned int height, unsigned int bytesPerPixel)
		: byteMap(byteMap), width(width), height(height), bytesPerPixel(bytesPerPixel) {}
	~ImageData(){ delete byteMap; }
};

}
