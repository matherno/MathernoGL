#include <texture/Texture.h>

namespace mathernogl {

void Texture::cleanUp(){
	glDeleteTextures(1, &glTexID);
}

}
