#pragma once

#include <GL/glew.h>

#include <mathernogl/data/GPUBuffer.h>

namespace mathernogl {

class VertexArray {
private:
	unsigned int glID = 0;

public:
	void init();
	void cleanUp();
	void linkBufferAsFloats(const GPUBuffer& buffer, unsigned int numComponents, unsigned int attribIndex, bool perInstance);
	void linkBufferAsFloats(const GPUBuffer& buffer, unsigned int byteOffset, unsigned int byteGap, unsigned int numComponents, unsigned int attribIndex, bool perInstance);
	void bind() const;
	void unbind() const;
};

}
