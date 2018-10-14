//
// Created by matt on 4/06/18.
//

#include "mathernogl/maths/GeometricOperations.h"
#include "mathernogl/maths/VectorOperations.h"

namespace mathernogl
  {

bool isPointInRect(const Vector2D& rectMin, const Vector2D& rectMax, const Vector2D& point)
  {
  return point.x >= rectMin.x && point.x <= rectMax.x && point.y >= rectMin.y && point.y <= rectMax.y;
  }

bool doesRectCircleIntersect(const Vector2D& rectMin, const Vector2D& rectMax, const Vector2D& circleCentroid, float circleRadius)
  {
  if (isPointInRect(rectMin, rectMax, circleCentroid))
    return true;
  const Vector2D& corner1 = rectMin;
  const Vector2D& corner2 = Vector2D(rectMin.x, rectMax.y);
  const Vector2D& corner3 = rectMax;
  const Vector2D& corner4 = Vector2D(rectMax.x, rectMin.y);
  if (doesLineCircleIntersect(corner1, corner2, circleCentroid, circleRadius))
    return true;
  if (doesLineCircleIntersect(corner2, corner3, circleCentroid, circleRadius))
    return true;
  if (doesLineCircleIntersect(corner3, corner4, circleCentroid, circleRadius))
    return true;
  if (doesLineCircleIntersect(corner4, corner1, circleCentroid, circleRadius))
    return true;
  return false;
  }

bool doesLineCircleIntersect(const Vector2D& lineStart, const Vector2D& lineEnd, const Vector2D& circleCentroid, float circleRadius)
  {
  Vector2D lineDir = lineEnd - lineStart;
  Vector2D centroidToStart = lineStart - circleCentroid;

  double a = dotProduct(lineDir, lineDir);
  double b = 2 * dotProduct(centroidToStart, lineDir);
  double c = dotProduct(centroidToStart, centroidToStart) - circleRadius * circleRadius;

  double discriminant = b * b - 4 * a * c;
  if (discriminant < 0)
    return false;
  discriminant = sqrt(discriminant);

  double t1 = (-b - discriminant) / (2 * a);
  double t2 = (-b + discriminant) / (2 * a);
  if (t1 >= 0 && t1 <= 1)
    return true;
  if (t2 >= 0 && t2 <= 1)
    return true;

  return false;
  }

  }
