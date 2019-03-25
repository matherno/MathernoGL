#pragma once
//
// Created by matt on 27/03/17.
//

#include <vector>

#include <data/GPUBuffer.h>
#include <Types.h>

namespace mathernogl {

class GPUBufferDynamic : public GPUBuffer {
private:
  uint numBytesAllocated = 0;

public:
  void allocateMemory(uint numBytes);
  void copyDataFloat(const std::vector<float>& data);
};

}


