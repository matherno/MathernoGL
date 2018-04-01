#pragma once

#include <GL/glew.h>
#include <mathernogl/image/ImageData.h>
#include <memory>


namespace mathernogl {

class Texture {
public:
	const unsigned int glTexID;
	const unsigned int glTexType;
  const uint width;
  const uint height;
  const uint bytesPerPixel;
  std::unique_ptr<byte> bytes;

  Texture(uint glTexID, uint glTexType, uint width, uint height, uint bytesPerPixel)
    : glTexID(glTexID), glTexType(glTexType), width(width), height(height), bytesPerPixel(bytesPerPixel) {}
	void cleanUp();
};

}
