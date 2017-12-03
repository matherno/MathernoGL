#pragma once

#include <stdexcept>
#include <GL/glew.h>

#include <mathernogl/utils/Logging.h>
#include <mathernogl/utils/ImageImporter.h>
#include <mathernogl/image/ImageData.h>
#include <mathernogl/texture/Texture.h>

namespace mathernogl {

enum TextureFiltering {
	NEAREST, LINEAR
};

enum TextureWrapping {
	REPEAT, REPEAT_MIRRORED, CLAMP_EDGE, CLAMP_BORDER
};

Texture* createTextureFromFile(const std::string& filePath, bool genMipMaps);
Texture* createTextureFromFile(const std::string& filePath, bool genMipMaps, TextureFiltering filtering, TextureWrapping wrapping);

}
