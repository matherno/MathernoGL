#pragma once
//
// Created by matherno on 1/10/17.
//

#include <mathernogl/maths/Matrix4.h>
#include <mathernogl/maths/Vector3D.h>

namespace mathernogl{

class Transform {
private:
  Matrix4 transformMatrix;

public:
  Transform();
  Transform(const Transform& transform);
  Transform(const Matrix4& transformMatrix);

  const Matrix4* getTransformMatrix() const { return & transformMatrix; }
  void setTransformMatrix(const Matrix4& transformMatrix);
  void setIdentityMatrix();
  bool isIdentity() const;  //  no transformation if this returns true
  void invert();
  Transform getInverse() const;

  void translate(float x, float y, float z);
  void translate(const Vector3D& translation);
  void rotate(float axisX, float axisY, float axisZ, float angle);
  void rotate(const Vector3D& axis, float angle);
  void scale(float scale);
  void scale(float scaleX, float scaleY, float scaleZ);
  void scale(const Vector3D& scale);

  Vector3D transform(const Vector3D& point) const;
  void transform(const Vector3D& point, Vector3D* transformedPoint) const;
  void transform(Vector3D* point) const;
  void transform(Vector3D* point, int num) const;
  void transformNormal(Vector3D* normal) const;
  Vector3D transformNormal(const Vector3D& normal) const;
};

}
