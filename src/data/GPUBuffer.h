#pragma once

#include <GL/glew.h>

namespace mathernogl {

class GPUBuffer {
protected:
	unsigned int glBufferID = 0;

public:
	void init();
	void cleanUp();
	void bind() const;
	void unbind() const;
};

}
