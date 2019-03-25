//
// Created by matt on 24/12/17.
//

#include "RenderableTerrain.h"

using namespace mathernogl;

RenderableTerrain::RenderableTerrain(uint id, uint numCells, float cellSize, int drawStage)
    : Renderable(id, drawStage), numCells(numCells), cellSize(cellSize)
  {

  }

RenderableTerrain::RenderableTerrain(uint id, std::shared_ptr<HeightMap>& heightMap, float cellSize)
    : Renderable(id), heightMap(heightMap), cellSize(cellSize)
  {
  numCells = (heightMap->width - 1);
  }

void RenderableTerrain::initialise(RenderContext* renderContext)
  {
  std::vector<Shader> shaders = { Shader(GL_VERTEX_SHADER, "shaders/MeshVS.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/MeshFS.glsl") };
  shaderProgram = renderContext->getSharedShaderProgram(&shaders);
  setDepthTest(true);

  meshStorage = renderContext->createEmptyMeshStorage();
  if (heightMap)
    mathernogl::createGridHeightMapped(numCells, numCells, cellSize, &heightMap->heights, &meshStorage->vertices);
  else
    mathernogl::createGrid(numCells, numCells, cellSize, &meshStorage->vertices);

  Vector3D colour;
  for (int vertNum = 0; vertNum < meshStorage->vertices.size(); ++vertNum)
    {
    if (vertNum % 3 == 0)
      colour = mathernogl::lerpVector3D(baseColour1, baseColour2, mathernogl::RandomGenerator::randomFloat(0, 1));
    meshStorage->normals.emplace_back(0, 1, 0);
    meshStorage->colours.emplace_back(colour);
    }
  meshStorage->setUseIndices(false);
  meshStorage->initialiseVAO();
  }

void RenderableTerrain::cleanUp(RenderContext* renderContext)
  {
  meshStorage->cleanUp();
  }

void RenderableTerrain::render(RenderContext* renderContext)
  {
  renderContext->activateShaderProgram(shaderProgram);
  shaderProgram->setVarInt("inDrawStyle", MESH_DRAW_STYLE_VERT_COLOUR);
  shaderProgram->setVarFloat("inAlpha", 1);
  shaderProgram->setVarInt("inLightShaded", 1);
  meshStorage->getVAO().bind();
  setDepthTest(true);
  setAlphaBlending(false);
  setFaceCulling(false);
  glDrawArrays(GL_TRIANGLES, 0, meshStorage->getNumVertices());
  }

void RenderableTerrain::setSingleColour(const Vector3D& colour)
  {
  setMultiColour(colour, colour);
  }

void RenderableTerrain::setMultiColour(const Vector3D& baseColour1, const Vector3D& baseColour2)
  {
  this->baseColour1 = baseColour1;
  this->baseColour2 = baseColour2;
  }

float RenderableTerrain::getHeightAt(const Vector3D& worldPos) const
  {
  const Vector3D objectPos = getTransform()->getInverse().transform(worldPos);
  if (heightMap)
    {
    int topLeftCol = (int)floor(objectPos.x / cellSize);
    int topLeftRow = (int)floor(objectPos.z / cellSize);
    float avgHeight = 0;
    for (int colNum = 0; colNum < 2; ++colNum)
      {
      for (int rowNum = 0; rowNum < 2; ++rowNum)
        {
        int col = topLeftCol + colNum;
        int row = topLeftRow + rowNum;
        if (col >= 0 && col <= numCells && row >= 0 && row <= numCells)
          avgHeight += heightMap->getHeight((uint)col, (uint)row);
        else
          return TERRAIN_OUT_OF_BOUNDS;
        }
      }

    return avgHeight / 4;
    }
  else
    {
    const float terrainWidth = cellSize * numCells;
    if (objectPos.x >= 0 && objectPos.z >= 0 && objectPos.x <= terrainWidth && objectPos.z <= terrainWidth)
      return 0;
    else
      return TERRAIN_OUT_OF_BOUNDS;
    }
  }

