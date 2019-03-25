//
// Created by matt on 23/12/18.
//

#include "VoxelStorage.h"


static const std::vector<float> voxelVerts =
  {
    0.5,  -0.5, -0.5,
    -0.5, -0.5, -0.5,
    0.5,  -0.5, 0.5,
    -0.5, -0.5, -0.5,
    -0.5, -0.5, 0.5,
    0.5,  -0.5, 0.5,

    0.5,  0.5, -0.5,
    -0.5, 0.5, -0.5,
    0.5,  0.5, 0.5,
    -0.5, 0.5, -0.5,
    -0.5, 0.5, 0.5,
    0.5,  0.5, 0.5,

    0.5,  0.5,  -0.5,
    0.5,  0.5,  0.5,
    0.5,  -0.5, -0.5,
    0.5,  0.5,  0.5,
    0.5,  -0.5, 0.5,
    0.5,  -0.5, -0.5,

    -0.5,  0.5,  -0.5,
    -0.5,  -0.5, -0.5,
    -0.5,  0.5,  0.5,
    -0.5,  -0.5, -0.5,
    -0.5,  -0.5, 0.5,
    -0.5,  0.5,  0.5,

    0.5,  -0.5, 0.5,
    0.5,  0.5,  0.5,
    -0.5, -0.5, 0.5,
    0.5,  0.5,  0.5,
    -0.5, 0.5,  0.5,
    -0.5, -0.5, 0.5,

    0.5,  -0.5, -0.5,
    -0.5, -0.5, -0.5,
    0.5,  0.5,  -0.5,
    -0.5, -0.5, -0.5,
    -0.5, 0.5,  -0.5,
    0.5,  0.5,  -0.5,
  };

static const std::vector<float> voxelNorms =
  {
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,

    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,

    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,

    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,

    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,

    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
  };

bool VoxelStorage::initialiseVAO()
  {
  numVoxels = 0;

  mathernogl::clearGLErrors();

  mathernogl::GPUBufferStatic vertexBuffer;
  vertexBuffer.init();
  vertexBuffer.copyDataFloat(voxelVerts);

  mathernogl::GPUBufferStatic normalBuffer;
  normalBuffer.init();
  normalBuffer.copyDataFloat(voxelNorms);


  std::vector<float> perVoxelData;
  for (const mathernogl::VoxelPoint& voxel : voxels)
    {
    perVoxelData.emplace_back(voxel.x);
    perVoxelData.emplace_back(voxel.y);
    perVoxelData.emplace_back(voxel.z);
    perVoxelData.emplace_back(voxel.col);
    ++numVoxels;
    }
  mathernogl::GPUBufferStatic perVoxelBuffer;
  perVoxelBuffer.init();
  perVoxelBuffer.copyDataFloat(perVoxelData);

  vao.init();
  vao.bind();
  vao.linkBufferAsFloats(vertexBuffer, 3, 0, false);
  vao.linkBufferAsFloats(normalBuffer, 3, 1, false);
  vao.linkBufferAsFloats(perVoxelBuffer, 4, 2, true);
  vao.unbind();

  vertexBuffer.cleanUp();
  normalBuffer.cleanUp();
  perVoxelBuffer.cleanUp();

  ASSERT_NO_GL_ERROR();
  return glGetError() == GL_NO_ERROR;
  }

void VoxelStorage::cleanUp()
  {
  vao.cleanUp();
  numVoxels = 0;
  }

void VoxelStorage::getMin(int& x, int& y, int& z)
  {
  if (!minMaxValid)
    recalculateMinMax();
  x = min.x;
  y = min.y;
  z = min.z;
  }

void VoxelStorage::getMax(int& x, int& y, int& z)
  {
  if (!minMaxValid)
    recalculateMinMax();
  x = max.x;
  y = max.y;
  z = max.z;
  }

mathernogl::Vector3D VoxelStorage::getMin()
  {
  if (!minMaxValid)
    recalculateMinMax();
  mathernogl::Vector3D vecMin;
  vecMin.x = min.x;
  vecMin.y = min.y;
  vecMin.z = min.z;
  return vecMin;
  }

mathernogl::Vector3D VoxelStorage::getMax()
  {
  if (!minMaxValid)
    recalculateMinMax();
  mathernogl::Vector3D vecMax;
  vecMax.x = max.x;
  vecMax.y = max.y;
  vecMax.z = max.z;
  return vecMax;
  }


void VoxelStorage::recalculateMinMax()
  {
  bool isFirst = true;
  for (mathernogl::VoxelPoint& voxel : voxels)
    {
    if (isFirst)
      {
      min = voxel;
      max = voxel;
      isFirst = false;
      }
    else
      {
      min.x = std::min(min.x, voxel.x);
      min.y = std::min(min.y, voxel.y);
      min.z = std::min(min.z, voxel.z);
      max.x = std::max(max.x, voxel.x);
      max.y = std::max(max.y, voxel.y);
      max.z = std::max(max.z, voxel.z);
      }
    }

  minMaxValid = true;
  }

void VoxelStorage::invalidateMinMax()
  {
  minMaxValid = false;
  }
