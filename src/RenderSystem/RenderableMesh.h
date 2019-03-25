#pragma once
//
// Created by matherno on 19/11/17.
//


#include "RenderSystem.h"
#include "MeshStorage.h"

class RenderableMesh : public Renderable
  {
protected:
  MeshStoragePtr meshStorage;
  ShaderProgramPtr shaderProgram;
  TexturePtr texture;
  float textureColourMix = 0;
  bool lightShaded = true;
  Vector3D colour = Vector3D(0.2, 0.5, 0.6);
  float transparency = 0;
  bool backFaceCulling = true;
  bool wireframeMode = false;
  int drawStyle;
  int cellShadedGrades = 0;
  BoundingBoxPtr objSpaceBounds;
  BoundingBoxPtr worldSpaceBounds;
  Transform boundingBoxTransform;

public:
  RenderableMesh(uint id, int drawStage = DRAW_STAGE_OPAQUE);

  virtual void initialise(RenderContext* renderContext) override;
  virtual void cleanUp(RenderContext* renderContext) override;
  virtual void render(RenderContext* renderContext) override;
  virtual void renderShadowMap(RenderContext* renderContext) override;
  void render(RenderContext* renderContext, bool shadowMap);

  void setMeshStorage(MeshStoragePtr meshStorage) { this->meshStorage = meshStorage; }
  MeshStoragePtr getMeshStorage() const { return meshStorage; }
  void setTransparency(float transparency) { this->transparency = transparency; }
  void setBackFaceCulling(bool backFaceCulling) { this->backFaceCulling = backFaceCulling; }
  void setWireframeMode(bool enable) { wireframeMode = enable; }
  void setLightShaded(bool lightShaded){ this->lightShaded = lightShaded; }
  void setCellShaded(bool flatShaded, int numCellGrades = 6){ this->cellShadedGrades = flatShaded ? numCellGrades : 0; }

  void setDrawStyleSingleColour(Vector3D colour);
  void setDrawStyleTexture(TexturePtr texture);
  void setDrawStyleTexture(TexturePtr texture, float colourMixFactor, const Vector3D& colourMix);
  void setDrawStyleVertColours();

  virtual BoundingBoxPtr getBounds() override;
  };
