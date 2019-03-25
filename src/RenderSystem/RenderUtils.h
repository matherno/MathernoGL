#pragma once

/*
*   
*/

#include "BoundingBox.h"

namespace RenderUtils
  {

  void transformBoundingBox(BoundingBox* bounds, const mathernogl::Transform* transform);
  bool isBoundingBoxClipped(const BoundingBox* bounds, const mathernogl::Matrix4* worldToClipTransform);

  };
