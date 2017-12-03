#include <mathernogl/data/GPUBufferStatic.h>

namespace mathernogl {

void GPUBufferStatic::copyDataFloat(const std::vector<float>& data){
	bind();
	glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), data.data(), GL_STATIC_DRAW);
	unbind();
}

}
