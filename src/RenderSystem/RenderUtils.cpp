//
// Created by matt on 25/01/19.
//

#include "RenderUtils.h"
#include "maths/Maths.h"

using namespace mathernogl;

void RenderUtils::transformBoundingBox(BoundingBox* bounds, const mathernogl::Transform* transform)
  {
  std::vector<Vector3D> corners;
  bounds->getCorners(&corners);

  bool isFirst = true;
  for (Vector3D& corner : corners)
    {
    transform->transform(corner, &corner);
    if (isFirst)
      bounds->setBounds(corner, corner);
    else
      bounds->addPoint(corner);
    isFirst = false;
    }
  }

static bool does1DLinesIntersect(double a1, double a2, double b1, double b2)
  {
  if (a1 > a2)
    std::swap(a1, a2);
  if (b1 > b2)
    std::swap(b1, b2);
  if (a1 < b1 && a2 < b1)
    return false;
  if (a1 > b2 && a2 > b2)
    return false;
  return true;
  }

bool RenderUtils::isBoundingBoxClipped(const BoundingBox* bounds, const mathernogl::Matrix4* worldToClipTransform)
  {
  std::vector<Vector3D> corners;
  bounds->getCorners(&corners);
  std::vector<Vector3D> clipSpaceCorners;
  for (const Vector3D& corner : corners)
    {
    const Vector3D clipCorner = corner * *worldToClipTransform;
    if(clipCorner.x > -1 && clipCorner.x < 1 &&
      clipCorner.y > -1 && clipCorner.y < 1 &&
      clipCorner.z > -1 && clipCorner.z < 1)
      return false;
    clipSpaceCorners.push_back(clipCorner);
    }

  Vector3D clipMin, clipMax;
  for (int idx = 0; idx < clipSpaceCorners.size(); ++idx)
    {
    const Vector3D& clipCorner = clipSpaceCorners[idx];
    if (idx == 0)
      {
      clipMin = clipCorner;
      clipMax = clipCorner;
      }
    else
      {
      clipMin.x = std::min(clipMin.x, clipCorner.x);
      clipMin.y = std::min(clipMin.y, clipCorner.y);
      clipMin.z = std::min(clipMin.z, clipCorner.z);
      clipMax.x = std::max(clipMax.x, clipCorner.x);
      clipMax.y = std::max(clipMax.y, clipCorner.y);
      clipMax.z = std::max(clipMax.z, clipCorner.z);
      }
    }

  if (!does1DLinesIntersect(clipMin.x, clipMax.x, -1, 1))
    return true;
  if (!does1DLinesIntersect(clipMin.y, clipMax.y, -1, 1))
    return true;
  if (!does1DLinesIntersect(clipMin.z, clipMax.z, -1, 1))
    return true;

  return false;
  }
