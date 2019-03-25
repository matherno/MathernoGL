#pragma once
//
// Created by matherno on 19/11/17.
//

#include "maths/Maths.h"
#include "data/VertexArray.h"
#include "data/GPUBufferDynamic.h"
#include "data/GPUBufferStatic.h"
#include <memory>
#include "Assert.h"
#include "utils/GLUtils.h"

#define MESH_DRAW_STYLE_SINGLE_COLOUR 1
#define MESH_DRAW_STYLE_TEXTURE       2
#define MESH_DRAW_STYLE_VERT_COLOUR   3

class MeshStorage
  {
private:
  const uint id;
  mathernogl::VertexArray vao;
  int numVertices = 0;
  bool useIndices = true;
  mathernogl::Vector3D min;
  mathernogl::Vector3D max;
  bool minMaxValid = false;
  bool storingTexCoords = false;
  bool coloursPerFace = false;

public:
  std::vector<int> indices;
  std::vector<mathernogl::Vector3D> vertices;
  std::vector<mathernogl::Vector3D> normals;
  std::vector<mathernogl::Vector2D> texCoords;
  std::vector<mathernogl::Vector3D> colours;

  MeshStorage(const uint id) : id(id) {};
  virtual bool initialiseVAO();
  mathernogl::VertexArray getVAO() { return vao; }
  int getNumVertices() const { return numVertices; }
  virtual void cleanUp();
  void setUseIndices(bool use){ useIndices = use; }
  void invalidateMinMax();
  mathernogl::Vector3D getMin();
  mathernogl::Vector3D getMax();
  bool gotTexCoords() const { return storingTexCoords; }
  uint getID() const { return id; }
  void setColoursPerFace(bool perFace) { coloursPerFace = perFace; }

protected:
  void recalculateMinMax();
  bool initVAOWithTexCoords(std::vector<int>* indicesPtr);
  bool initVAOWithColours(std::vector<int>* indicesPtr);
  };

class MeshStorageInstanced : public MeshStorage
  {
private:
  const uint maxNumInstances;
  mathernogl::GPUBufferDynamic instanceBuffer;
  uint numInstancesInBuffer = 0;
  std::vector<float> bufferData;

public:
  MeshStorageInstanced(const uint id, uint maxNumInstances) : MeshStorage(id), maxNumInstances(maxNumInstances) {};
  virtual bool initialiseVAO() override;
  virtual void cleanUp() override;
  void updateInstanceBuffer();
  void clearInstances();
  void addInstance(const mathernogl::Matrix4* transform);
  uint getNumInstances() { return numInstancesInBuffer; }
  };

typedef std::shared_ptr<MeshStorage> MeshStoragePtr;
typedef std::shared_ptr<MeshStorageInstanced> MeshStorageInstancedPtr;

