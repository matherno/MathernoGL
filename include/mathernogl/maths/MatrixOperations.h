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
Matrix4 matrixTranslate(double x, double y, double z);
Matrix4 matrixRotate(const Vector3D& rotationAxis, double rotationAngle);
Matrix4 matrixRotate(double x, double y, double z, double rotationAngle);
Matrix4 matrixRotateBetween(const Vector3D& from, const Vector3D& to);
Matrix4 matrixPitch(double angle);
Matrix4 matrixYaw(double angle);
Matrix4 matrixRoll(double angle);
Matrix4 matrixScale(const Vector3D& scaling);
Matrix4 matrixScale(double x, double y, double z);
Matrix4 matrixScale(double scaling);

Matrix4 matrixPerspective(double fov, double aspectRatio, double zNearPLane, double zFarPlane);
Matrix4 matrixOrthogonal(double fov, double aspectRatio, double zFarPlane);

Matrix4 matrixInverse(const Matrix4& matrix);

}
