#pragma once

#include "RenderSystem.h"


/*
*
*/

class VoxelBatch;
class VoxelBatchManager;

class VoxelBatchLink : public Renderable
  {
private:
  friend class VoxelBatch;
  friend class VoxelBatchManager;
  uint batchID = 0;
  string voxelModelFilePath;
  Vector3D transform;
  BoundingBoxPtr bounds;
  uint voxelCount = 0;

public:
  VoxelBatchLink(uint id);
  virtual void initialise(RenderContext* renderContext) override;
  virtual void cleanUp(RenderContext* renderContext) override;
  virtual void render(RenderContext* renderContext) override;
  virtual BoundingBoxPtr getBounds() override;

  };
typedef std::shared_ptr<VoxelBatchLink> VoxelBatchLinkPtr;


struct VoxelModelData
  {
  string filePath;
  Vector3D min;
  Vector3D max;
  std::vector<mathernogl::VoxelPoint> voxels;
  };


class VoxelBatch : public Renderable
  {
private:
  mathernogl::MappedList<VoxelBatchLinkPtr> voxelModelInstances;
  bool perInstanceBufferInvalid = true;
  uint voxelCount = 0;
  mathernogl::VertexArray vao;
  mathernogl::GPUBufferStatic perInstanceBuffer;
  ShaderProgramPtr shaderProgram;
  BoundingBoxPtr bounds;

public:
  VoxelBatch(uint id);
  virtual void initialise(RenderContext* renderContext) override;
  virtual void cleanUp(RenderContext* renderContext) override;
  virtual void render(RenderContext* renderContext) override;
  virtual void renderShadowMap(RenderContext* renderContext) override;
  virtual BoundingBoxPtr getBounds() override;

  VoxelBatchLinkPtr createInstance(RenderContext* renderContext, VoxelModelData* modelData, const Vector3D& position);
  VoxelBatchLinkPtr getInstance(uint instanceID);
  void removeInstance(uint instanceID);

protected:
  void refreshInstanceBuffer(RenderContext* renderContext);
  };
typedef std::shared_ptr<VoxelBatch> VoxelBatchPtr;


class VoxelBatchManager
  {
private:
  typedef std::pair<int, int> GridIndexPair;
  std::map<string, VoxelModelData> voxelModels;
  std::vector<Vector3D> coloursArray;
  double voxelSize = 1;
  double batchSize = 1;
  std::map<GridIndexPair, VoxelBatchPtr> batchesMap;
  std::map<uint, VoxelBatchPtr> batchesIDMap;

public:
  VoxelBatchLinkPtr createVoxelModelInstance(RenderContext* renderContext, const string& filePath, const Vector3D& position);
  void removeVoxelModelInstance(VoxelBatchLink* link);
  void cleanUp(RenderContext* renderContext);
  void setBatchSize(double size) { batchSize = size; }
  void setVoxelSize(double size) { voxelSize = size; }

  const VoxelModelData* getModelData(const string& filePath) const;
  const std::vector<mathernogl::Vector3D>* getColoursArray() const { return &coloursArray; }
  double getVoxelSize() const { return voxelSize; }
  double getBatchSize() const { return batchSize; }

protected:
  VoxelModelData* createSharedModelData(const string& filePath);
  uint addColour(const mathernogl::Vector3D& colour);
  void calculateMinMax(VoxelModelData& model);
  };