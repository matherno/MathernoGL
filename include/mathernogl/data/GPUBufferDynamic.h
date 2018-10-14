#pragma once
//
// Created by matt on 27/03/17.
//

#include <vector>

#include <mathernogl/data/GPUBuffer.h>
#include <mathernogl/Types.h>

namespace mathernogl {

class GPUBufferDynamic : public GPUBuffer {
private:
  uint numBytesAllocated = 0;

public:
  void allocateMemory(uint numBytes);
  void copyDataFloat(const std::vector<float>& data);
};

}


