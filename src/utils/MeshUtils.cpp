//
// Created by matherno on 3/09/17.
//

#include <mathernogl/utils/Logging.h>
#include <mathernogl/utils/MeshUtils.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace mathernogl {


void loadObj(std::string filePath, std::vector<int>* indices, std::vector<Vector3D>* verts, std::vector<Vector3D>* norms, std::vector<Vector2D>* texCoords, bool onlyFirstShape /*= true*/) {
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string error;

  if (!tinyobj::LoadObj(shapes, materials, error, filePath.c_str())) {
    throw std::runtime_error("Couldn't load obj at '" + filePath + "' ");
  }

  indices->clear();
  verts->clear();
  norms->clear();
  texCoords->clear();

  for(int shapeNum = 0; shapeNum < shapes.size(); ++shapeNum) {
    std::vector<uint>& inIndices = shapes[shapeNum].mesh.indices;
    std::vector<float>& inVerts = shapes[shapeNum].mesh.positions;
    std::vector<float>& inNorms = shapes[shapeNum].mesh.normals;
    std::vector<float>& inTexCoords = shapes[shapeNum].mesh.texcoords;

    //  vertex indices
    for (uint vertexIndex : inIndices) {
      indices->emplace_back(vertexIndex);
    }

    //  vertex positions
    if (inVerts.size() > 0) {
      for (int vertexNum = 0; vertexNum < inVerts.size(); vertexNum += 3) {
        verts->emplace_back(inVerts[vertexNum], inVerts[vertexNum + 1], inVerts[vertexNum + 2]);
      }
    }
    else
      throw std::runtime_error("Could not load obj. No vertex position data found in '" + filePath + "'!");

    //  vertex normals
    if (inNorms.size() > 0) {
      for (int vertexNum = 0; vertexNum < inNorms.size(); vertexNum += 3) {
        norms->emplace_back(inNorms[vertexNum], inNorms[vertexNum + 1], inNorms[vertexNum + 2]);
      }
    }
    else {
//      logWarning("No vertex normal data found in '" + filePath + "'!");
      for (int vertexNum = 0; vertexNum < inVerts.size(); vertexNum += 3) {
        Vector3D normal(inVerts[vertexNum], inVerts[vertexNum + 1], inVerts[vertexNum + 2]);
        normal.makeUniform();
        norms->push_back(normal);
      }
    }

    // vertex texture coordinates
    if (inTexCoords.size() > 0) {
      for (int vertexNum = 0; vertexNum < inTexCoords.size(); vertexNum += 2) {
        texCoords->emplace_back(inTexCoords[vertexNum], inTexCoords[vertexNum + 1]);
      }
    }
    else {
//      logWarning("No vertex texture coordinate data found in '" + filePath + "'!");
    }

    if(onlyFirstShape)
      break;
  }
}

//creates an axis aligned cube with it's centre at <0,0,0> and given width
void createCube(float width, std::vector<Vector3D>* verts) {
  width /= 2;
  verts->push_back( { -width, width, -width });
  verts->push_back( { -width, -width, -width });
  verts->push_back( { width, -width, -width });
  verts->push_back( { width, -width, -width });
  verts->push_back( { width, width, -width });
  verts->push_back( { -width, width, -width });

  verts->push_back( { -width, -width, width });
  verts->push_back( { -width, -width, -width });
  verts->push_back( { -width, width, -width });
  verts->push_back( { -width, width, -width });
  verts->push_back( { -width, width, width });
  verts->push_back( { -width, -width, width });

  verts->push_back( { width, -width, -width });
  verts->push_back( { width, -width, width });
  verts->push_back( { width, width, width });
  verts->push_back( { width, width, width });
  verts->push_back( { width, width, -width });
  verts->push_back( { width, -width, -width });

  verts->push_back( { -width, -width, width });
  verts->push_back( { -width, width, width });
  verts->push_back( { width, width, width });
  verts->push_back( { width, width, width });
  verts->push_back( { width, -width, width });
  verts->push_back( { -width, -width, width });

  verts->push_back( { -width, width, -width });
  verts->push_back( { width, width, -width });
  verts->push_back( { width, width, width });
  verts->push_back( { width, width, width });
  verts->push_back( { -width, width, width });
  verts->push_back( { -width, width, -width });

  verts->push_back( { -width, -width, -width });
  verts->push_back( { -width, -width, width });
  verts->push_back( { width, -width, -width });
  verts->push_back( { width, -width, -width });
  verts->push_back( { -width, -width, width });
  verts->push_back( { width, -width, width });
}

//creates an axis aligned cube with the given bounds
void createCube(float minX, float maxX, float minY, float maxY, float minZ, float maxZ, std::vector<Vector3D>* verts) {
  verts->push_back( { minX, maxY, minZ });
  verts->push_back( { maxX, minY, minZ });
  verts->push_back( { minX, minY, minZ });
  verts->push_back( { maxX, minY, minZ });
  verts->push_back( { minX, maxY, minZ });
  verts->push_back( { maxX, maxY, minZ });

  verts->push_back( { minX, minY, maxZ });
  verts->push_back( { minX, maxY, minZ });
  verts->push_back( { minX, minY, minZ });
  verts->push_back( { minX, maxY, minZ });
  verts->push_back( { minX, minY, maxZ });
  verts->push_back( { minX, maxY, maxZ });

  verts->push_back( { maxX, minY, minZ });
  verts->push_back( { maxX, maxY, maxZ });
  verts->push_back( { maxX, minY, maxZ });
  verts->push_back( { maxX, maxY, maxZ });
  verts->push_back( { maxX, minY, minZ });
  verts->push_back( { maxX, maxY, minZ });

  verts->push_back( { minX, minY, maxZ });
  verts->push_back( { maxX, maxY, maxZ });
  verts->push_back( { minX, maxY, maxZ });
  verts->push_back( { maxX, maxY, maxZ });
  verts->push_back( { minX, minY, maxZ });
  verts->push_back( { maxX, minY, maxZ });

  verts->push_back( { minX, maxY, minZ });
  verts->push_back( { maxX, maxY, maxZ });
  verts->push_back( { maxX, maxY, minZ });
  verts->push_back( { maxX, maxY, maxZ });
  verts->push_back( { minX, maxY, minZ });
  verts->push_back( { minX, maxY, maxZ });

  verts->push_back( { minX, minY, minZ });
  verts->push_back( { maxX, minY, minZ });
  verts->push_back( { minX, minY, maxZ });
  verts->push_back( { maxX, minY, minZ });
  verts->push_back( { maxX, minY, maxZ });
  verts->push_back( { minX, minY, maxZ });
}

void createGrid(uint numCellsX, uint numCellsY, float cellSize, std::vector<Vector3D>* verts) {
  for (int segNumX = 0; segNumX < numCellsX; segNumX++) {
    for (int segNumY = 0; segNumY < numCellsY; segNumY++) {
      const Vector3D topLeft = Vector3D(segNumX * cellSize, 0, segNumY * cellSize);
      const Vector3D topRight = topLeft + Vector3D(cellSize, 0, 0);
      const Vector3D bottomLeft = topLeft + Vector3D(0, 0, cellSize);
      const Vector3D bottomRight = topLeft + Vector3D(cellSize, 0, cellSize);

      verts->push_back(topLeft);
      verts->push_back(bottomLeft);
      verts->push_back(bottomRight);
      verts->push_back(topLeft);
      verts->push_back(bottomRight);
      verts->push_back(topRight);
    }
  }
}

#define GET_HEIGHT(map, width, col, row) (*map)[(col) + (width) * (row)]

void createGridHeightMapped(uint numCellsX, uint numCellsY, float cellSize, std::vector<float>* heightMap, std::vector<Vector3D>* verts) {
  for (int segNumX = 0; segNumX < numCellsX; segNumX++) {
    for (int segNumY = 0; segNumY < numCellsY; segNumY++) {
      Vector3D topLeft = Vector3D(segNumX * cellSize, 0, segNumY * cellSize);
      Vector3D topRight = topLeft + Vector3D(cellSize, 0, 0);
      Vector3D bottomLeft = topLeft + Vector3D(0, 0, cellSize);
      Vector3D bottomRight = topLeft + Vector3D(cellSize, 0, cellSize);

      topLeft.y = GET_HEIGHT(heightMap, numCellsX+1, segNumX, segNumY);
      topRight.y = GET_HEIGHT(heightMap, numCellsX+1, segNumX+1, segNumY);
      bottomLeft.y = GET_HEIGHT(heightMap, numCellsX+1, segNumX, segNumY+1);
      bottomRight.y = GET_HEIGHT(heightMap, numCellsX+1, segNumX+1, segNumY+1);

      verts->push_back(topLeft);
      verts->push_back(bottomLeft);
      verts->push_back(bottomRight);
      verts->push_back(topLeft);
      verts->push_back(bottomRight);
      verts->push_back(topRight);
      }
    }
  }

}
