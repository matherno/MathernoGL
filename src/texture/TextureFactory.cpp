#include <mathernogl/texture/TextureFactory.h>
#include <memory>

namespace mathernogl {

void setupFilteringWrapping(unsigned int textureType, TextureFiltering filtering, TextureWrapping wrapping);
void setupMipmapping(unsigned int textureType, float textureLODBias);

Texture* createTextureFromFile(const std::string& filePath, const TextureOptions& options) {
	Texture* texture = nullptr;
	try{
		std::unique_ptr<ImageData> imageData(loadImage(filePath));

		unsigned int glTexType = GL_TEXTURE_2D;
		unsigned int glTexID  = createGLTexture(glTexType, imageData->width, imageData->height, imageData->bytesPerPixel, options, false, imageData->byteMap);
		texture = new Texture(glTexID, glTexType, imageData->width, imageData->height, imageData->bytesPerPixel, false);

    const unsigned int numBytes = imageData->width * imageData->height * imageData->bytesPerPixel;
    texture->bytes.reset(new byte[numBytes]);
    std::memcpy(texture->bytes.get(), imageData->byteMap, numBytes);
	}
	catch(const std::runtime_error& e){
		std::string error = e.what();
		throw std::runtime_error("Couldn't create texture at '" + filePath + "'. " + error);
	}
	return texture;
}

Texture* createEmptyTexture(uint width, uint height, uint bytesPerPixel, const TextureOptions& options){
  const unsigned int numBytes = width * height * bytesPerPixel;
  std::unique_ptr<byte> bytes(new byte[numBytes]);
  std::fill(bytes.get(), bytes.get() + numBytes, 0);

  unsigned int glTexType = GL_TEXTURE_2D;
  unsigned int glTexID = createGLTexture(glTexType, width, height, bytesPerPixel, options, true, bytes.get());

  Texture* texture = new Texture(glTexID, glTexType, width, height, bytesPerPixel, true);
  texture->bytes = std::move(bytes);
  return texture;
  }

Texture* createEmptyAlphaTexture(uint width, uint height, const TextureOptions& options){
  return createEmptyTexture(width, height, 1, options);
}

Texture* createDepthTexture(uint width, uint height){
  unsigned int glTexType = GL_TEXTURE_2D;
  unsigned int glTexID;
  glGenTextures(1, &glTexID);
  glBindTexture(glTexType, glTexID);
  glTexImage2D(glTexType, 0, GL_DEPTH_COMPONENT24, (GLsizei)width, (GLsizei)height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
  setupFilteringWrapping(glTexType, NEAREST, CLAMP_EDGE);
  return new Texture(glTexID, glTexType, width, height, 0, true);
}

Texture* createMultiSampleTexture(uint width, uint height, uint numSamples, bool gammaCorrect) {
  unsigned int glTexType = GL_TEXTURE_2D_MULTISAMPLE;
  unsigned int glTexID;
  glGenTextures(1, &glTexID);
  glBindTexture(glTexType, glTexID);
  glTexImage2DMultisample(glTexType, numSamples, gammaCorrect ? GL_SRGB8_ALPHA8 : GL_RGBA, width, height, GL_TRUE);
  return new Texture(glTexID, glTexType, width, height, 0, true);
}

Texture* createShadowMapDepthTexture(uint width, uint height)
  {
  unsigned int glTexType = GL_TEXTURE_2D;
  unsigned int glTexID;
  glGenTextures(1, &glTexID);
  glBindTexture(glTexType, glTexID);
  glTexStorage2D(glTexType, 1, GL_DEPTH_COMPONENT32, (GLsizei)width, (GLsizei)height);
  setupFilteringWrapping(glTexType, LINEAR, CLAMP_EDGE);
  glTexParameteri(glTexType, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(glTexType, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  return new Texture(glTexID, glTexType, width, height, 0, true);
  }

uint getGLBytesFormat(uint bytesPerPixel, bool rgbFormat){
  switch (bytesPerPixel){
    case 3:
      return rgbFormat ? GL_RGB : GL_BGR;
    case 4:
      return rgbFormat ? GL_RGBA : GL_BGRA;
    case 1:
      return GL_RED;
    default:
      throw std::runtime_error("Incompatible bit depth for creating gl texture, at " + std::to_string(bytesPerPixel * 8) + ", only support for 8, 24, and 32 bit ");
  }
}

void updateTexture(Texture* texture) {
  const uint glFormat = getGLBytesFormat(texture->bytesPerPixel, texture->rgbFormat);
  glBindTexture(texture->glTexType, texture->glTexID);
  glTexSubImage2D(texture->glTexType, 0, 0, 0, texture->width, texture->height, glFormat, GL_UNSIGNED_BYTE, (GLvoid*)texture->bytes.get());
}

uint createGLTexture(uint glTexType, uint width, uint height, uint bytesPerPixel, const TextureOptions& options, bool rgbFormat, const byte* byteData){
  uint glTexID;
  glGenTextures(1, &glTexID);
  glBindTexture(glTexType, glTexID);
  setupFilteringWrapping(glTexType, options.filtering, options.wrapping);

  const uint glInternalFormat = options.gammaCorrect ? GL_SRGB8_ALPHA8 : GL_RGBA;
  const uint glFormat = getGLBytesFormat(bytesPerPixel, rgbFormat);
  glTexImage2D(glTexType, 0, glInternalFormat, (GLsizei) width, (GLsizei) height, 0, glFormat, GL_UNSIGNED_BYTE, byteData);
  if(options.genMipMaps)
    setupMipmapping(glTexType, -0.7);
  glBindTexture(glTexType, 0);
  return glTexID;
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
