//
// Created by matherno on 3/09/17.
//

#include <utils/Logging.h>
#include <utils/MeshUtils.h>
#include <utils/Misc.h>
#include <set>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace mathernogl {


void loadObj(std::string filePath, std::vector<int>* indices, std::vector<Vector3D>* verts, std::vector<Vector3D>* norms, std::vector<Vector2D>* texCoords, std::vector<Vector3D>* faceCols /*= nullptr*/, bool onlyFirstShape /*= true*/) {
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string error;

  std::string objDirectory = mathernogl::getDirectoryFromPath(filePath);
  if (!tinyobj::LoadObj(shapes, materials, error, filePath.c_str(), objDirectory.c_str())) {
    throw std::runtime_error("Couldn't load obj at '" + filePath + "' ");
  }

  indices->clear();
  verts->clear();
  norms->clear();
  texCoords->clear();
  if (faceCols)
    faceCols->clear();

  long vertexIdxOffset = 0;
  for(int shapeNum = 0; shapeNum < shapes.size(); ++shapeNum) {
    std::vector<uint>& inIndices = shapes[shapeNum].mesh.indices;
    std::vector<float>& inVerts = shapes[shapeNum].mesh.positions;
    std::vector<float>& inNorms = shapes[shapeNum].mesh.normals;
    std::vector<float>& inTexCoords = shapes[shapeNum].mesh.texcoords;
    std::vector<int>& inMaterials = shapes[shapeNum].mesh.material_ids;

    vertexIdxOffset = verts->size();

    //  vertex indices
    for (uint vertexIndex : inIndices) {
      indices->emplace_back(vertexIndex + vertexIdxOffset);
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

    if (faceCols && !inMaterials.empty()) {
      for (int materialNum : inMaterials) {
        if (materialNum >= 0 && materialNum < materials.size())
          {
          auto material = materials[materialNum];
          faceCols->emplace_back(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
          }
        else
          {
          faceCols->emplace_back(0, 0, 0);
          }
      }
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


#define MGV_IDENTIFIER "mgv"
#define MGV_COLOURS "colour"
#define MGV_VOXELS "voxels"

void loadVoxelMGVFile(const std::string& filePath, std::vector<Vector3D>* colours, std::vector<VoxelPoint>* voxels, bool includeInteriorVoxels)
  {
  std::ifstream infile(filePath);
  std::string line;
  if (!std::getline(infile, line))
    {
    mathernogl::logWarning("Can not read file: " + filePath);
    return;
    }

  if (line != MGV_IDENTIFIER)
    {
    mathernogl::logWarning("Unrecognised voxel file: " + filePath);
    return;
    }

  std::getline(infile, line); //  version num (don't need atm)

  std::list<VoxelPoint> unfilteredVoxels;
  std::set<std::tuple<int, int, int>> voxelSet;

  bool readingColours = true;
  while(std::getline(infile, line))
    {
    if (line.empty())
      continue;
    if (line == MGV_COLOURS)
      continue;
    if (line == MGV_VOXELS)
      {
      readingColours = false;
      continue;
      }

    if (readingColours)
      {
      std::vector<std::string> splits = stringSplit(line, ',');
      if (splits.size() >= 3)
        {
        int r = atoi(splits[0].c_str());
        int g = atoi(splits[1].c_str());
        int b = atoi(splits[2].c_str());
        colours->emplace_back((double)r / 255.0, (double)g / 255.0, (double)b / 255.0);
        }
      }
    else
      {
      std::vector<std::string> splits = stringSplit(line, ',');
      if (splits.size() >= 4)
        {
        int x = atoi(splits[0].c_str());
        int y = atoi(splits[1].c_str());
        int z = atoi(splits[2].c_str());
        int col = atoi(splits[3].c_str());
        if (voxelSet.count(std::make_tuple(x, y, z)) == 0)
          {
          if (!includeInteriorVoxels)
            unfilteredVoxels.emplace_back(x, y, z, col);
          else
            voxels->emplace_back(x, y, z, col);
          voxelSet.insert(std::make_tuple(x, y, z));
          }
        }
      }
    }

  if (!includeInteriorVoxels)
    {
    for (VoxelPoint voxel : unfilteredVoxels)
      {
      int adjacentVoxels = 0;
      for (VoxelPoint voxel2 : unfilteredVoxels)
        {
        const int distanceAway = abs(voxel.x - voxel2.x) + abs(voxel.y - voxel2.y) + abs(voxel.z - voxel2.z);
        if (distanceAway == 1)
          ++adjacentVoxels;
        }
      if (adjacentVoxels < 6)
        voxels->emplace_back(voxel);
      }
    }
  }



}


