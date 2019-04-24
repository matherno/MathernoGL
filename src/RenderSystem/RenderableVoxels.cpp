//
// Created by matt on 23/12/18.
//

#include "RenderableVoxels.h"
#include "RenderUtils.h"
#include "imgui/imgui.h"

RenderableVoxels::RenderableVoxels(uint id, int drawStage) : Renderable(id, drawStage)
  {

  }

void RenderableVoxels::initialise(RenderContext* renderContext)
  {
  using namespace mathernogl;
  std::vector<Shader> shaders = { Shader(GL_VERTEX_SHADER, "shaders/VoxelVS.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/VoxelFS.glsl") };
  shaderProgram = renderContext->getSharedShaderProgram(&shaders);
  }

void RenderableVoxels::cleanUp(RenderContext* renderContext)
  {

  }

void RenderableVoxels::render(RenderContext* renderContext)
  {
  mathernogl::setDepthTest(true);
  mathernogl::clearGLErrors();
  renderContext->activateShaderProgram(shaderProgram);
  mathernogl::setFaceCulling(backFaceCulling);

  int colIdx = 0;
  for (Vector3D col : storage->colours)
    {
    float h, s, v, r = col.x, g = col.y, b = col.z;
    ImGui::ColorConvertRGBtoHSV(r, g, b, h, s, v);
    s *= colSaturationFactor;
    v *= colValueFactor;
    ImGui::ColorConvertHSVtoRGB(h, s, v, r, g, b);
    shaderProgram->setVarVec3Array("inColours", colIdx++, Vector3D(r, g, b));
    }
  shaderProgram->setVarFloat("inVoxelSize", voxelSize);

  storage->getVAO().bind();
  glDrawArraysInstanced(GL_TRIANGLES, 0, 36, storage->getNumVoxels());
  ASSERT_NO_GL_ERROR();
  }

void RenderableVoxels::renderShadowMap(RenderContext* renderContext)
  {
  render(renderContext);
  }

void RenderableVoxels::setVoxelStorage(VoxelStoragePtr storage)
  {
  this->storage = storage;
  }

void RenderableVoxels::setVoxelSize(double size)
  {
  voxelSize = size;
  }

void RenderableVoxels::setSaturationFactor(double factor)
  {
  colSaturationFactor = factor;
  }

void RenderableVoxels::setValueFactor(double factor)
  {
  colValueFactor = factor;
  }

BoundingBoxPtr RenderableVoxels::getBounds()
  {
  if (!objSpaceBounds && storage)
    {
    Vector3D worldMin = storage->getMin() * voxelSize;
    Vector3D worldMax = storage->getMax() * voxelSize;
    worldMax.x += voxelSize;
    worldMax.y += voxelSize;
    worldMax.z += voxelSize;
    objSpaceBounds = std::make_shared<BoundingBox>(worldMin, worldMax);
    }

  if (boundingBoxTransform.getTransformMatrix() != getTransform()->getTransformMatrix())
    {
    worldSpaceBounds = nullptr;
    boundingBoxTransform = *getTransform();
    }

  if (objSpaceBounds && !worldSpaceBounds)
    {
    worldSpaceBounds = std::make_shared<BoundingBox>(*objSpaceBounds);
    RenderUtils::transformBoundingBox(worldSpaceBounds.get(), getTransform());
    }
  return worldSpaceBounds;
  }

void RenderableVoxels::setBackFaceCulling(bool enable)
  {
  backFaceCulling = enable;
  }
