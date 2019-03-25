#pragma once

/*
*   
*/

#include "Vector2D.h"

namespace mathernogl
  {

bool isPointInRect(const Vector2D& rectMin, const Vector2D& rectMax, const Vector2D& point);
bool doesRectCircleIntersect(const Vector2D& rectMin, const Vector2D& rectMax, const Vector2D& circleCentroid, float circleRadius);
bool doesLineCircleIntersect(const Vector2D& lineStart, const Vector2D& lineEnd, const Vector2D& circleCentroid, float circleRadius);

  }