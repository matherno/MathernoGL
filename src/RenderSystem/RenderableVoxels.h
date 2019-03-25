#pragma once

#include "RenderSystem.h"
#include "VoxelStorage.h"

/*
*   
*/

class RenderableVoxels : public Renderable
  {
private:
  VoxelStoragePtr storage;
  ShaderProgramPtr shaderProgram;
  double voxelSize = 1;
  BoundingBoxPtr objSpaceBounds;
  BoundingBoxPtr worldSpaceBounds;
  Transform boundingBoxTransform;
  bool backFaceCulling = true;

public:
  RenderableVoxels(uint id, int drawStage = DRAW_STAGE_OPAQUE);
  virtual void initialise(RenderContext* renderContext) override;
  virtual void cleanUp(RenderContext* renderContext) override;
  virtual void render(RenderContext* renderContext) override;
  virtual void renderShadowMap(RenderContext* renderContext) override;
  virtual BoundingBoxPtr getBounds() override;

  void setVoxelStorage(VoxelStoragePtr storage);
  void setVoxelSize(double size);
  void setBackFaceCulling(bool enable);

  VoxelStoragePtr getVoxelStorage() { return storage; }
  double getVoxelSize() const { return voxelSize; }
  };
