//
// Created by matherno on 19/11/17.
//

#include "MeshStorage.h"

#define SHADER_LOCATION_VERT  0
#define SHADER_LOCATION_NORM  1
#define SHADER_LOCATION_COL   2
#define SHADER_LOCATION_TEX   3
#define SHADER_LOCATION_TRANS1   4
#define SHADER_LOCATION_TRANS2   5
#define SHADER_LOCATION_TRANS3   6
#define SHADER_LOCATION_TRANS4   7

using namespace mathernogl;

bool MeshStorage::initialiseVAO()
  {
  numVertices = 0;
  if (vertices.empty() || normals.empty())
    return false;

  std::vector<int>* indicesPtr = &indices;
  std::vector<int> tempIndices;
  if (useIndices)
    {
    if (indices.empty())
      return false;
    }
  else
    {
    int index = 0;
    for (const Vector3D& vert : vertices)
      tempIndices.push_back(index++);
    indicesPtr = &tempIndices;
    }

  if (!texCoords.empty())
    return initVAOWithTexCoords(indicesPtr);
  else
    return initVAOWithColours(indicesPtr);
  }

void MeshStorage::cleanUp()
  {
  vao.cleanUp();
  numVertices = 0;
  }

bool MeshStorage::initVAOWithTexCoords(std::vector<int>* indicesPtr)
  {
  std::vector<float> bufferData;
  for (const int& index : *indicesPtr)
    {
    Vector3D& vert = vertices[index];
    Vector3D& norm = normals[index];
    Vector2D tex(0);
    if (!texCoords.empty())
      tex = texCoords[index];
    bufferData.emplace_back(vert.x);
    bufferData.emplace_back(vert.y);
    bufferData.emplace_back(vert.z);
    bufferData.emplace_back(norm.x);
    bufferData.emplace_back(norm.y);
    bufferData.emplace_back(norm.z);
    bufferData.emplace_back(tex.x);
    bufferData.emplace_back(tex.y);
    ++numVertices;
    }

  mathernogl::clearGLErrors();
  GPUBufferStatic buffer;
  buffer.init();
  buffer.copyDataFloat(bufferData);
  vao.init();
  vao.bind();
  vao.linkBufferAsFloats(buffer, 0 * sizeof(float), 5 * sizeof(float), 3, SHADER_LOCATION_VERT, false);
  vao.linkBufferAsFloats(buffer, 3 * sizeof(float), 5 * sizeof(float), 3, SHADER_LOCATION_NORM, false);
  vao.linkBufferAsFloats(buffer, 6 * sizeof(float), 6 * sizeof(float), 2, SHADER_LOCATION_TEX, false);
  vao.unbind();
  buffer.cleanUp();
  ASSERT_NO_GL_ERROR();

  storingTexCoords = true;
  return glGetError() == GL_NO_ERROR;
  }

bool MeshStorage::initVAOWithColours(std::vector<int>* indicesPtr)
  {
  std::vector<float> bufferData;
  int idxNum = 0;
  for (const int& index : *indicesPtr)
    {
    Vector3D& vert = vertices[index];
    Vector3D& norm = normals[index];
    Vector3D col(0.8, 0.2, 0.6);
    if (!colours.empty())
      {
      if (coloursPerFace)
        col = colours[(int)((float)idxNum / 3)];
      else
        col = colours[index];
      }
    bufferData.emplace_back(vert.x);
    bufferData.emplace_back(vert.y);
    bufferData.emplace_back(vert.z);
    bufferData.emplace_back(norm.x);
    bufferData.emplace_back(norm.y);
    bufferData.emplace_back(norm.z);
    bufferData.emplace_back(col.x);
    bufferData.emplace_back(col.y);
    bufferData.emplace_back(col.z);
    ++numVertices;
    ++idxNum;
    }

  mathernogl::clearGLErrors();
  GPUBufferStatic buffer;
  buffer.init();
  buffer.copyDataFloat(bufferData);
  vao.init();
  vao.bind();
  vao.linkBufferAsFloats(buffer, 0 * sizeof(float), 6 * sizeof(float), 3, SHADER_LOCATION_VERT, false);
  vao.linkBufferAsFloats(buffer, 3 * sizeof(float), 6 * sizeof(float), 3, SHADER_LOCATION_NORM, false);
  vao.linkBufferAsFloats(buffer, 6 * sizeof(float), 6 * sizeof(float), 3, SHADER_LOCATION_COL, false);
  vao.unbind();
  buffer.cleanUp();
  ASSERT_NO_GL_ERROR();

  storingTexCoords = false;
  return glGetError() == GL_NO_ERROR;
  }

mathernogl::Vector3D MeshStorage::getMin()
  {
  if (!minMaxValid)
    recalculateMinMax();
  return min;
  }

mathernogl::Vector3D MeshStorage::getMax()
  {
  if (!minMaxValid)
    recalculateMinMax();
  return max;
  }

void MeshStorage::recalculateMinMax()
  {
  bool firstVert = true;
  for (Vector3D& vert : vertices)
    {
    if (firstVert)
      {
      min = vert;
      max = vert;
      firstVert = false;
      }
    else
      {
      min.x = std::min(min.x, vert.x);
      min.y = std::min(min.y, vert.y);
      min.z = std::min(min.z, vert.z);
      max.x = std::max(max.x, vert.x);
      max.y = std::max(max.y, vert.y);
      max.z = std::max(max.z, vert.z);
      }
    }
  minMaxValid = true;
  }

void MeshStorage::invalidateMinMax()
  {
  minMaxValid = false;
  }


/*
 * MeshStorageInstanced
 */

#define FLOATS_PER_INSTANCE 12

bool MeshStorageInstanced::initialiseVAO()
  {
  if(MeshStorage::initialiseVAO())
    {
    instanceBuffer.init();
    instanceBuffer.allocateMemory(maxNumInstances * FLOATS_PER_INSTANCE * sizeof(float));
    getVAO().bind();
    getVAO().linkBufferAsFloats(instanceBuffer, 0 * sizeof(float), 9 * sizeof(float), 3, SHADER_LOCATION_TRANS1, true);
    getVAO().linkBufferAsFloats(instanceBuffer, 3 * sizeof(float), 9 * sizeof(float), 3, SHADER_LOCATION_TRANS2, true);
    getVAO().linkBufferAsFloats(instanceBuffer, 6 * sizeof(float), 9 * sizeof(float), 3, SHADER_LOCATION_TRANS3, true);
    getVAO().linkBufferAsFloats(instanceBuffer, 9 * sizeof(float), 9 * sizeof(float), 3, SHADER_LOCATION_TRANS4, true);
    getVAO().unbind();
    return true;
    }
  return false;
  }

void MeshStorageInstanced::cleanUp()
  {
  MeshStorage::cleanUp();
  instanceBuffer.cleanUp();
  }

void MeshStorageInstanced::updateInstanceBuffer()
  {
  numInstancesInBuffer = (uint)floor(bufferData.size() / FLOATS_PER_INSTANCE);
  instanceBuffer.copyDataFloat(bufferData);
  }

void MeshStorageInstanced::clearInstances()
  {
  bufferData.clear();
  }

void MeshStorageInstanced::addInstance(const mathernogl::Matrix4* transform)
  {
  if ((uint)floor(bufferData.size() / FLOATS_PER_INSTANCE) >= maxNumInstances)
    return;

  for (int row = 0; row < 4; ++row)
    {
    for (int col = 0; col < 3; ++col)
      bufferData.push_back((float)transform->getAt(row, col));
    }
  }
