#pragma once

#include <vector>

#include <mathernogl/data/GPUBuffer.h>

namespace mathernogl {

class GPUBufferStatic : public GPUBuffer {
public:
	void copyDataFloat(const std::vector<float>& data);
};

}

