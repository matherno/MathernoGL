#pragma once

#include <cmath>

#include <mathernogl/maths/Vector2D.h>
#include <mathernogl/maths/Vector3D.h>
#include <mathernogl/maths/Vector4D.h>
#include <mathernogl/maths/Matrix4.h>
#include <mathernogl/maths/VectorOperations.h>

namespace mathernogl {

Matrix4 matrixIdentity();

Matrix4 matrixTranslate(const Vector3D& translation);
Matrix4 matrixTranslate(const float x, const float y, const float z);
Matrix4 matrixRotate(const Vector3D& rotationAxis, const float rotationAngle);
Matrix4 matrixRotate(const float x, const float y, const float z, const float rotationAngle);
Matrix4 matrixPitch(float angle);
Matrix4 matrixYaw(float angle);
Matrix4 matrixRoll(float angle);
Matrix4 matrixScale(const Vector3D& scaling);
Matrix4 matrixScale(const float x, const float y, const float z);
Matrix4 matrixScale(const float scaling);

Matrix4 matrixPerspective(const float fov, const float aspectRatio, float zNearPLane, float zFarPlane);
Matrix4 matrixOrthogonal(const float fov, const float aspectRatio, const float zFarPlane);

Matrix4 matrixInverse(const Matrix4& matrix);

}
