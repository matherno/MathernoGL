#include <Assert.h>
#include "data/GPUBufferDynamic.h"

namespace mathernogl {

void GPUBufferDynamic::allocateMemory(uint numBytes){
  bind();
  glBufferData(GL_ARRAY_BUFFER, numBytes, 0, GL_DYNAMIC_DRAW);
  numBytesAllocated = numBytes;
  unbind();
}

void GPUBufferDynamic::copyDataFloat(const std::vector<float>& data){
  bind();
  uint bytesRequired = (uint) data.size() * sizeof(float);
  ASSERT(bytesRequired <= numBytesAllocated, "Don't have enough memory allocated! Data: " + std::to_string(bytesRequired) + " bytes. Allocated: " + std::to_string(numBytesAllocated) + " bytes.");
  if (bytesRequired <= numBytesAllocated){
    glBufferSubData(GL_ARRAY_BUFFER, 0, bytesRequired, data.data());
  unbind();
  }
}


}
