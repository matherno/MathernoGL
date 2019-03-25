//
// Created by matt on 5/01/18.
//

#include <maths/NoiseGenerator.h>
#include "HeightMapFactory.h"

void HeightMapFactory::createFlatMap(HeightMap* heightMap, uint width, float height)
  {
  for (uint col = 0; col < width; ++col)
    {
    for (uint row = 0; row < width; ++row)
      {
      heightMap->heights.push_back(height);
      }
    }
  heightMap->width = width;
  }

void HeightMapFactory::createNoiseMap(HeightMap* heightMap, uint width, float amplitude, float waveLength)
  {
  mathernogl::NoiseGenerator noiserX;
  noiserX.setWaveLength(waveLength);
  noiserX.setBounds(-amplitude, amplitude);
  mathernogl::NoiseGenerator noiserZ;
  noiserZ.setWaveLength(waveLength);
  noiserZ.setBounds(-amplitude, amplitude);
  for (uint col = 0; col < width; ++col)
    {
    for (uint row = 0; row < width; ++row)
      {
      heightMap->heights.push_back((noiserX.getNoise(col) + noiserZ.getNoise(row)) / 2.0f);
      }
    }
  heightMap->width = width;
  }

void HeightMapFactory::createDiamondSquareMap(HeightMap* heightMap, uint numPasses, float initSpread, float spreadReductionFactor)
  {
  if (numPasses < 1)
    {
    numPasses = 1;
    }
  const int heightMapWidth = (int) pow(2, numPasses) + 1;

  mathernogl::RandomGenerator random;
  heightMap->heights = std::vector<float>(heightMapWidth*heightMapWidth, 0);
  heightMap->width = (uint)heightMapWidth;
  std::vector<float>& heights = heightMap->heights;
  float spread = initSpread;
  random.setBounds(-spread / 2.0f, spread / 2.0f);
  heights[0] = 0;
  heights[heightMapWidth - 1] = 0;
  heights[(heightMapWidth - 1) * heightMapWidth] = 0;
  heights[(heightMapWidth - 1) + (heightMapWidth - 1) * heightMapWidth] = 0;
  for (int passNum = 0; passNum < numPasses; passNum++)
    {
    const int chunkWidth = (int) pow(2, numPasses - passNum) + 1;
    const int halfWidth = chunkWidth / 2;

    //diamond step
    for (int centrePointRow = halfWidth; centrePointRow < heightMapWidth; centrePointRow += chunkWidth - 1)
      {
      for (int centrePointCol = halfWidth; centrePointCol < heightMapWidth; centrePointCol += chunkWidth - 1)
        {
        const int topLeftIndex = centrePointCol - halfWidth + (centrePointRow + halfWidth) * heightMapWidth;
        const int topRightIndex = centrePointCol + halfWidth + (centrePointRow + halfWidth) * heightMapWidth;
        const int bottomLeftIndex = centrePointCol - halfWidth + (centrePointRow - halfWidth) * heightMapWidth;
        const int bottomRightIndex = centrePointCol + halfWidth + (centrePointRow - halfWidth) * heightMapWidth;
        const int centreIndex = centrePointCol + centrePointRow * heightMapWidth;
        heights[centreIndex] = heights[topLeftIndex] + heights[topRightIndex] + heights[bottomLeftIndex] + heights[bottomRightIndex];
        heights[centreIndex] /= 4.0f;
        heights[centreIndex] += random.getFloatValue();
        }
      }

    //square step
    bool offsetColByHalfWidth = true;
    for (int squarePointRow = 0; squarePointRow < heightMapWidth; squarePointRow += halfWidth)
      {
      int initCol = 0;
      if (offsetColByHalfWidth)
        {
        initCol = halfWidth;
        }
      offsetColByHalfWidth = !offsetColByHalfWidth;
      for (int squarePointCol = initCol; squarePointCol < heightMapWidth; squarePointCol += chunkWidth - 1)
        {
        int totalSummed = 0;
        float leftHeight = 0;
        float rightHeight = 0;
        float topHeight = 0;
        float bottomHeight = 0;
        if (squarePointRow != 0)
          {
          topHeight = heights[squarePointCol + (squarePointRow - halfWidth) * heightMapWidth];
          totalSummed++;
          }
        if (squarePointRow != heightMapWidth - 1)
          {
          bottomHeight = heights[squarePointCol + (squarePointRow + halfWidth) * heightMapWidth];
          totalSummed++;
          }
        if (squarePointCol != 0)
          {
          leftHeight = heights[squarePointCol - halfWidth + (squarePointRow) * heightMapWidth];
          totalSummed++;
          }
        if (squarePointCol != heightMapWidth - 1)
          {
          rightHeight = heights[squarePointCol + halfWidth + (squarePointRow) * heightMapWidth];
          totalSummed++;
          }
        heights[squarePointCol + squarePointRow * heightMapWidth] = (leftHeight + rightHeight + topHeight + bottomHeight) / (float) totalSummed + random.getFloatValue();
        }
      }

    //reduce random spread
    spread *= spreadReductionFactor;
    random.setBounds(-spread / 2.0f, spread / 2.0f);
    }
  }
