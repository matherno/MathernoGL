#pragma once

#include <cmath>

#include <maths/Vector2D.h>
#include <maths/Vector3D.h>
#include <maths/Vector4D.h>

namespace mathernogl {

//dot product returns a value that is 0 when the two input vectors are perpendicular
double dotProduct(const Vector2D& vector1, const Vector2D& vector2);
double dotProduct(const Vector3D& vector1, const Vector3D& vector2);
double dotProduct(const Vector4D& vector1, const Vector4D& vector2);

//cross product returns a vector that is perpendicular to the two input vectors, following right hand rule for direction
Vector3D crossProduct(const Vector3D& vector1, const Vector3D& vector2);

double angleBetween(const Vector2D& vector1, const Vector2D& vector2);
double angleBetween(const Vector3D& vector1, const Vector3D& vector2);
double angleBetween(const Vector4D& vector1, const Vector4D& vector2);

//returns the reflection of the given vector around the given normal
Vector3D reflect(const Vector3D& vector, const Vector3D& normal);

double ccwAngleBetween(const Vector2D& first, const Vector2D& second);
Vector3D lerpVector3D(const Vector3D& first, const Vector3D& second, double factor);

}
