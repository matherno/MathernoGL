#pragma once

#include <vector>
#include "utils/MeshUtils.h"
#include "data/VertexArray.h"
#include "data/GPUBufferStatic.h"
#include <memory>
#include "utils/GLUtils.h"
#include "Assert.h"

/*
*   
*/

class VoxelStorage
  {
private:
  const uint id;
  mathernogl::VertexArray vao;
  uint numVoxels = 0;
  mathernogl::VoxelPoint min;
  mathernogl::VoxelPoint max;
  bool minMaxValid = false;

public:
  std::vector<mathernogl::Vector3D> colours;
  std::vector<mathernogl::VoxelPoint> voxels;

  VoxelStorage(uint id) : id(id) {}
  bool initialiseVAO();
  void cleanUp();
  mathernogl::VertexArray getVAO() { return vao; }
  uint getID() const { return id; }
  uint getNumVoxels() const { return numVoxels; }
  void getMin(int& x, int& y, int& z);
  void getMax(int& x, int& y, int& z);
  mathernogl::Vector3D getMin();
  mathernogl::Vector3D getMax();
  void invalidateMinMax();

protected:
  void recalculateMinMax();
  };
typedef std::shared_ptr<VoxelStorage> VoxelStoragePtr;
