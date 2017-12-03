#pragma once

#include <iostream>
#include <stdexcept>
#include <cstring>
#include <FreeImage.h>

#include <mathernogl/image/ImageData.h>

namespace mathernogl {

ImageData* loadImage(const std::string &filePath);
void saveImage(const std::string &filePath, const ImageData* imageData);
void flipImageBothWays(byte* image, unsigned int width, unsigned int height, unsigned int bpp);
void flipImageVertically(byte* image, unsigned int width, unsigned int height, unsigned int bpp);
void flipImageHorizontally(byte* image, unsigned int width, unsigned int height, unsigned int bpp);

}
