#pragma once

#include <cmath>

#include <mathernogl/maths/Vector2D.h>
#include <mathernogl/maths/Vector3D.h>
#include <mathernogl/maths/Vector4D.h>

namespace mathernogl {

//dot product returns a value that is 0 when the two input vectors are perpendicular
float dotProduct(const Vector2D& vector1, const Vector2D& vector2);
float dotProduct(const Vector3D& vector1, const Vector3D& vector2);
float dotProduct(const Vector4D& vector1, const Vector4D& vector2);

//cross product returns a vector that is perpendicular to the two input vectors, following right hand rule for direction
Vector3D crossProduct(const Vector3D& vector1, const Vector3D& vector2);
Vector4D crossProduct(const Vector4D& vector1, const Vector4D& vector2);

float angleBetween(const Vector2D& vector1, const Vector2D& vector2);
float angleBetween(const Vector3D& vector1, const Vector3D& vector2);
float angleBetween(const Vector4D& vector1, const Vector4D& vector2);

//returns the reflection of the given vector around the given normal
mathernogl::Vector3D reflect(const mathernogl::Vector3D& vector, const mathernogl::Vector3D& normal);



}
