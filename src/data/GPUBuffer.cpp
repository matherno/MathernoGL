#include "mathernogl/data/GPUBuffer.h"

namespace mathernogl {

void GPUBuffer::init() {
	glGenBuffers(1, &glBufferID);
}

void GPUBuffer::cleanUp() {
	glDeleteBuffers(1, &glBufferID);
	glBufferID = 0;
}

void GPUBuffer::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, glBufferID);
}
void GPUBuffer::unbind() const {

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}
