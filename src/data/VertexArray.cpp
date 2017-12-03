#include <mathernogl/data/VertexArray.h>

namespace mathernogl {

void VertexArray::init() {
	glGenVertexArrays(1, &glID);
}

void VertexArray::cleanUp() {
	glDeleteVertexArrays(1, &glID);
	glID = 0;
}

//calls linkBufferAsFloats with 0 byteOffset and 0 byteGap
void VertexArray::linkBufferAsFloats(const GPUBuffer& buffer, unsigned int numComponents, unsigned int attribIndex, bool perInstance) {
	linkBufferAsFloats(buffer, 0, 0, numComponents, attribIndex, perInstance);
}

//links the buffer data as floats, given the byteOffset into the buffer to start, and the byteGap between consecutive data units of numComponents of floats
//use per instance if the given buffer data should divided per instance rather than per vertex
/*	eg
	unitData = numComponents * numFloatBytes
	|-byteOffset-|-unitData-|-byteGap-|-unitData-|-byteGap-|-unitData-|-byteGap-|-unitData-|- etc
 */
void VertexArray::linkBufferAsFloats(const GPUBuffer& buffer, unsigned int byteOffset, unsigned int byteGap, unsigned int numComponents, unsigned int attribIndex, bool perInstance) {
	buffer.bind();
	glEnableVertexAttribArray(attribIndex);
	glVertexAttribPointer(attribIndex, numComponents, GL_FLOAT, GL_FALSE, numComponents*sizeof(float)+byteGap, (GLvoid*)byteOffset);
	buffer.unbind();

	if (perInstance) {
		glVertexAttribDivisor(attribIndex, 1);
	}
	else {
		glVertexAttribDivisor(attribIndex, 0);
	}
}

void VertexArray::bind() const {
	glBindVertexArray(glID);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}

}
