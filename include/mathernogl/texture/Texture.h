#pragma once

#include <GL/glew.h>


namespace mathernogl {

class Texture {
public:
	const unsigned int glTexID;
	const unsigned int glTexType;

	Texture(unsigned int glTexID, unsigned int glTexType) : glTexID(glTexID), glTexType(glTexType) {}
	void cleanUp();
};

}
