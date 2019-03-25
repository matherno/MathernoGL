//
// Created by matt on 30/01/19.
//

#include "VoxelBatchManager.h"

#define FLOATS_PER_VOXEL_INSTANCE 4

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






VoxelBatchLink::VoxelBatchLink(uint id) : Renderable(id)
  {

  }

void VoxelBatchLink::initialise(RenderContext* renderContext)
  {

  }

void VoxelBatchLink::cleanUp(RenderContext* renderContext)
  {
  renderContext->getVoxelBatchManager()->removeVoxelModelInstance(this);
  }

void VoxelBatchLink::render(RenderContext* renderContext)
  {

  }

BoundingBoxPtr VoxelBatchLink::getBounds()
  {
  return bounds;
  }


VoxelBatch::VoxelBatch(uint id) : Renderable(id)
  {

  }

void VoxelBatch::initialise(RenderContext* renderContext)
  {
  mathernogl::clearGLErrors();

  using namespace mathernogl;
  std::vector<Shader> shaders = { Shader(GL_VERTEX_SHADER, "shaders/VoxelVS.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/VoxelFS.glsl") };
  shaderProgram = renderContext->getSharedShaderProgram(&shaders);

  voxelCount = 0;

  mathernogl::GPUBufferStatic vertexBuffer;
  vertexBuffer.init();
  vertexBuffer.copyDataFloat(voxelVerts);

  mathernogl::GPUBufferStatic normalBuffer;
  normalBuffer.init();
  normalBuffer.copyDataFloat(voxelNorms);

  perInstanceBuffer.init();

  vao.init();
  vao.bind();
  vao.linkBufferAsFloats(vertexBuffer, 3, 0, false);
  vao.linkBufferAsFloats(normalBuffer, 3, 1, false);
  vao.linkBufferAsFloats(perInstanceBuffer, FLOATS_PER_VOXEL_INSTANCE, 2, true);
  vao.unbind();

  vertexBuffer.cleanUp();
  normalBuffer.cleanUp();
  ASSERT_NO_GL_ERROR();
  }

void VoxelBatch::cleanUp(RenderContext* renderContext)
  {
  perInstanceBuffer.cleanUp();
  vao.cleanUp();
  perInstanceBufferInvalid = true;
  voxelModelInstances.clear();
  voxelCount = 0;
  }

void VoxelBatch::render(RenderContext* renderContext)
  {
  if (perInstanceBufferInvalid)
    refreshInstanceBuffer(renderContext);

  mathernogl::clearGLErrors();
  renderContext->activateShaderProgram(shaderProgram);
  mathernogl::setFaceCulling(true);

  int colIdx = 0;
  auto colours = renderContext->getVoxelBatchManager()->getColoursArray();
  for (const Vector3D& col : *colours)
    shaderProgram->setVarVec3Array("inColours", colIdx++, col);
  shaderProgram->setVarFloat("inVoxelSize", (float)renderContext->getVoxelBatchManager()->getVoxelSize());

  vao.bind();
  glDrawArraysInstanced(GL_TRIANGLES, 0, 36, voxelCount);
  ASSERT_NO_GL_ERROR();
  }

void VoxelBatch::renderShadowMap(RenderContext* renderContext)
  {
  if (perInstanceBufferInvalid)
    refreshInstanceBuffer(renderContext);

  mathernogl::clearGLErrors();
  renderContext->activateShaderProgram(shaderProgram);
  mathernogl::setFaceCulling(true);

  shaderProgram->setVarFloat("inVoxelSize", (float)renderContext->getVoxelBatchManager()->getVoxelSize());

  vao.bind();
  glDrawArraysInstanced(GL_TRIANGLES, 0, 36, voxelCount);
  ASSERT_NO_GL_ERROR();
  }

BoundingBoxPtr VoxelBatch::getBounds()
  {
  return bounds;
  }

VoxelBatchLinkPtr VoxelBatch::createInstance(RenderContext* renderContext, VoxelModelData* modelData, const mathernogl::Vector3D& position)
  {
  VoxelBatchLink* voxelModelInstance = new VoxelBatchLink(renderContext->getNextRenderableID());
  voxelModelInstance->transform = position;
  voxelModelInstance->batchID = getID();
  voxelModelInstance->bounds.reset(new BoundingBox(modelData->min + position, modelData->max + position));
  voxelModelInstance->voxelModelFilePath = modelData->filePath;
  voxelModelInstance->voxelCount = (uint) modelData->voxels.size();

  voxelCount += voxelModelInstance->voxelCount;

  if (!bounds)
    bounds.reset(new BoundingBox());
  bounds->addBoundingBox(voxelModelInstance->bounds);

  perInstanceBufferInvalid = true;
  VoxelBatchLinkPtr ptr(voxelModelInstance);
  voxelModelInstances.add(ptr, ptr->getID());
  return ptr;
  }

VoxelBatchLinkPtr VoxelBatch::getInstance(uint instanceID)
  {
  if (voxelModelInstances.contains(instanceID))
    return voxelModelInstances.get(instanceID);
  return nullptr;
  }

void VoxelBatch::removeInstance(uint instanceID)
  {
  if (voxelModelInstances.contains(instanceID))
    {
    voxelCount -= voxelModelInstances.get(instanceID)->voxelCount;
    voxelModelInstances.remove(instanceID);
    perInstanceBufferInvalid = true;

    bounds.reset(new BoundingBox());
    for (const VoxelBatchLinkPtr& modelInstance : *voxelModelInstances.getList())
      bounds->addBoundingBox(modelInstance->bounds);
    }
  }

void VoxelBatch::refreshInstanceBuffer(RenderContext* renderContext)
  {
  const VoxelBatchManager* voxelBatchManager = renderContext->getVoxelBatchManager();

  std::vector<float> voxelBufferData(voxelCount * FLOATS_PER_VOXEL_INSTANCE);
  int index = 0;
  for (const VoxelBatchLinkPtr& modelInstance : *voxelModelInstances.getList())
    {
    const VoxelModelData* modelData = voxelBatchManager->getModelData(modelInstance->voxelModelFilePath);
    for (const mathernogl::VoxelPoint& voxelPoint : modelData->voxels)
      {
      voxelBufferData[index++] = voxelPoint.x + (modelInstance->transform.x / voxelBatchManager->getVoxelSize());
      voxelBufferData[index++] = voxelPoint.y + (modelInstance->transform.y / voxelBatchManager->getVoxelSize());
      voxelBufferData[index++] = voxelPoint.z + (modelInstance->transform.z / voxelBatchManager->getVoxelSize());
      voxelBufferData[index++] = voxelPoint.col;
      }
    }

  perInstanceBuffer.copyDataFloat(voxelBufferData);

  perInstanceBufferInvalid = false;
  }






VoxelBatchLinkPtr VoxelBatchManager::createVoxelModelInstance(RenderContext* renderContext, const string& filePath, const Vector3D& position)
  {
  GridIndexPair gridIndexPair;
  gridIndexPair.first = (int) std::floor(position.x / batchSize);
  gridIndexPair.second = (int) std::floor(position.z / batchSize);

  VoxelBatchPtr batch;
  if (batchesMap.count(gridIndexPair) == 0)
    {
    batch.reset(new VoxelBatch(renderContext->getNextRenderableID()));
    batch->initialise(renderContext);
    renderContext->getRenderableSet()->addRenderable(batch);
    batchesMap[gridIndexPair] = batch;
    batchesIDMap[batch->getID()] = batch;
    }
  else
    {
    batch = batchesMap[gridIndexPair];
    }

  return batch->createInstance(renderContext, createSharedModelData(filePath), position);
  }

void VoxelBatchManager::cleanUp(RenderContext* renderContext)
  {
  for (auto pair : batchesMap)
    {
    VoxelBatchPtr batch = pair.second;
    renderContext->getRenderableSet()->removeRenderable(batch->getID());
    batch->cleanUp(renderContext);
    batch = nullptr;
    }

  batchesMap.clear();
  batchesIDMap.clear();
  }

void VoxelBatchManager::removeVoxelModelInstance(VoxelBatchLink* link)
  {
  batchesIDMap[link->batchID]->removeInstance(link->getID());
  }

const VoxelModelData* VoxelBatchManager::getModelData(const string& filePath) const
  {
  if(voxelModels.count(filePath) == 0)
    return nullptr;
  return &voxelModels.at(filePath);
  }

VoxelModelData* VoxelBatchManager::createSharedModelData(const string& filePath)
  {
  if (voxelModels.count(filePath) > 0)
    return &voxelModels[filePath];

  voxelModels[filePath] = VoxelModelData();
  VoxelModelData& model = voxelModels[filePath];
  std::vector<Vector3D> modelColours;
  loadVoxelMGVFile(filePath, &modelColours, &model.voxels, true);

  //  load in the colours, recording the new colour indexes
  std::map<uint, uint> loadedColIdxToNew;
  for (uint oldColIndex = 0; oldColIndex < modelColours.size(); ++oldColIndex)
    {
    const uint newColIndex = addColour(modelColours[oldColIndex]);
    loadedColIdxToNew[oldColIndex] = newColIndex;
    }

  //  update the voxel points with their new colour indexes
  for (mathernogl::VoxelPoint& voxelPoint : model.voxels)
    voxelPoint.col = loadedColIdxToNew[voxelPoint.col];

  model.filePath = filePath;
  calculateMinMax(model);
  return &voxelModels[filePath];
  }


uint VoxelBatchManager::addColour(const mathernogl::Vector3D& colour)
  {
  using namespace mathernogl;
  uint colIndex = 0;
  Vector3D colCompare = colour * 255;
  for (Vector3D storedColour : coloursArray)
    {
    storedColour *= 255;
    if ((byte)colCompare.x == (byte)storedColour.x && (byte)colCompare.y == (byte)storedColour.y && (byte)colCompare.z == (byte)storedColour.z)
      return colIndex;
    colIndex++;
    }

  colIndex = (uint)coloursArray.size();
  coloursArray.push_back(colour);
  ASSERT(coloursArray.size() <= 32, "More colours aren't supported as of yet");
  return colIndex;
  }

void VoxelBatchManager::calculateMinMax(VoxelModelData& model)
  {
  bool isFirst = true;
  Vector3D min;
  Vector3D max;
  for (mathernogl::VoxelPoint& voxel : model.voxels)
    {
    Vector3D voxelMin(voxel.x, voxel.y, voxel.z);
    Vector3D voxelMax = voxelMin + 1;

    if (isFirst)
      {
      min = voxelMin;
      max = voxelMax;
      isFirst = false;
      }
    else
      {
      min.x = std::min(min.x, voxelMin.x);
      min.y = std::min(min.y, voxelMin.y);
      min.z = std::min(min.z, voxelMin.z);
      max.x = std::max(max.x, voxelMax.x);
      max.y = std::max(max.y, voxelMax.y);
      max.z = std::max(max.z, voxelMax.z);
      }
    }

  model.min.set(min.x, min.y, min.z);
  model.max.set(max.x, max.y, max.z);
  model.min *= voxelSize;
  model.max *= voxelSize;
  }

