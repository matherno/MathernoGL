#include <mathernogl/texture/TextureFactory.h>
#include <memory>

namespace mathernogl {

void setupFilteringWrapping(unsigned int textureType, TextureFiltering filtering, TextureWrapping wrapping);
void setupMipmapping(unsigned int textureType, float textureLODBias);

Texture* createTextureFromFile(const std::string& filePath, bool genMipMaps) {
	return createTextureFromFile(filePath, genMipMaps, LINEAR, REPEAT);
}

Texture* createTextureFromFile(const std::string& filePath, bool genMipMaps, TextureFiltering filtering, TextureWrapping wrapping) {
	Texture* texture = nullptr;
	try{
		std::unique_ptr<ImageData> imageData(loadImage(filePath));

		unsigned int glTexType = GL_TEXTURE_2D;
		unsigned int glTexID;
		glGenTextures(1, &glTexID);
		glBindTexture(glTexType, glTexID);
		setupFilteringWrapping(glTexType, filtering, wrapping);

		if (imageData->bytesPerPixel == 3) {
			glTexImage2D(glTexType, 0, GL_RGBA, (GLsizei) imageData->width, (GLsizei) imageData->height, 0, GL_BGR, GL_UNSIGNED_BYTE, imageData->byteMap);
		}
		else if (imageData->bytesPerPixel == 4) {
			glTexImage2D(glTexType, 0, GL_RGBA, (GLsizei) imageData->width, (GLsizei) imageData->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, imageData->byteMap);
		}
		else if (imageData->bytesPerPixel == 1){
			glTexImage2D(glTexType, 0, GL_RGBA, (GLsizei) imageData->width, (GLsizei) imageData->height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, imageData->byteMap);
		}
		else{
			throw std::runtime_error("Incompatible bit depth for image bitmap at " + std::to_string(imageData->bytesPerPixel*8) + ", only support for 8, 24, and 32 bit: " + filePath);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_BGRA, GL_UNSIGNED_BYTE, imageBitmap);
		}
		if(genMipMaps){
			setupMipmapping(glTexType, -0.7);
		}
		glBindTexture(glTexType, 0);

		texture = new Texture(glTexID, glTexType, imageData->width, imageData->height, imageData->bytesPerPixel);
    const unsigned int numBytes = imageData->width * imageData->height * imageData->bytesPerPixel;
    texture->bytes.reset(new byte[numBytes]);
    std::memcpy(texture->bytes.get(), imageData->byteMap, numBytes);
	}
	catch(const std::runtime_error& e){
		std::string error = e.what();
		throw std::runtime_error("Couldn't create texture: " + error);
	}
	return texture;
}

Texture* createEmptyTexture(uint width, uint height, TextureFiltering filtering, TextureWrapping wrapping){
  const unsigned int bytesPerPixel = 3;
  const unsigned int numBytes = width * height * bytesPerPixel;
  std::unique_ptr<byte> bytes(new byte[numBytes]);
  std::fill(bytes.get(), bytes.get() + numBytes, 0);

  unsigned int glTexType = GL_TEXTURE_2D;
  unsigned int glTexID;
  glGenTextures(1, &glTexID);
  glBindTexture(glTexType, glTexID);
  setupFilteringWrapping(glTexType, filtering, wrapping);
  glTexImage2D(glTexType, 0, GL_RGB, (GLsizei) width, (GLsizei) height, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes.get());
  glBindTexture(glTexType, 0);
  Texture* texture = new Texture(glTexID, glTexType, width, height, bytesPerPixel);
  texture->bytes = std::move(bytes);
  return texture;
}

void updateTexture(Texture* texture) {
  glBindTexture(texture->glTexType, texture->glTexID);
  glTexSubImage2D(texture->glTexType, 0, 0, 0, texture->width, texture->height, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)texture->bytes.get());
}

  void setupFilteringWrapping(unsigned int textureType, TextureFiltering filtering, TextureWrapping wrapping){
	int glFiltering = GL_LINEAR;
	switch(filtering){
		case LINEAR:
			glFiltering = GL_LINEAR;
			break;

		case NEAREST:
			glFiltering = GL_NEAREST;
			break;
	}

	int glWrapping = GL_REPEAT;
	switch(wrapping){
		case REPEAT:
			glWrapping = GL_REPEAT;
			break;

		case REPEAT_MIRRORED:
			glWrapping = GL_MIRRORED_REPEAT;
			break;

		case CLAMP_EDGE:
			glWrapping = GL_CLAMP_TO_EDGE;
			break;

		case CLAMP_BORDER:
			glWrapping = GL_CLAMP_TO_BORDER;
			break;
	}

	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, glFiltering);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, glFiltering);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, glWrapping);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, glWrapping);
}

void setupMipmapping(unsigned int textureType, float textureLODBias){
	glGenerateMipmap(textureType);
	glTexParameterf(textureType, GL_TEXTURE_LOD_BIAS, textureLODBias);
	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

}
