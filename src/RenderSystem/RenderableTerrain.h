#pragma once

#include <RenderSystem/HeightMapFactory.h>
#include "RenderSystem.h"

#define TERRAIN_OUT_OF_BOUNDS std::numeric_limits<float>::max()

/*
*   
*/

class RenderableTerrain : public Renderable
  {
private:
  uint numCells;
  float cellSize;
  MeshStoragePtr meshStorage;
  ShaderProgramPtr shaderProgram;
  Vector3D baseColour1;
  Vector3D baseColour2;
  std::shared_ptr<HeightMap> heightMap;

public:
  RenderableTerrain(uint id, uint numCells, float cellSize, int drawStage = DRAW_STAGE_OPAQUE);
  RenderableTerrain(uint id, std::shared_ptr<HeightMap>& heightMap, float cellSize);

  virtual void initialise(RenderContext* renderContext) override;
  virtual void cleanUp(RenderContext* renderContext) override;
  virtual void render(RenderContext* renderContext) override;

  void setSingleColour(const Vector3D& colour);
  void setMultiColour(const Vector3D& baseColour1, const Vector3D& baseColour2);

  float getCellSize() const { return cellSize; }
  float getNumCellsAcross() const { return numCells; }
  float getHeightAt(const Vector3D& worldPos) const;
  };
