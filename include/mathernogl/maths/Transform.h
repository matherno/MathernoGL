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
  Vector3D getTranslationComponent() const;

  void translate(double x, double y, double z);
  void translate(const Vector3D& translation);
  void rotate(double axisX, double axisY, double axisZ, double angle);
  void rotate(const Vector3D& axis, double angle);
  void rotateBetween(const Vector3D& from, const Vector3D& to);
  void scale(double scale);
  void scale(double scaleX, double scaleY, double scaleZ);
  void scale(const Vector3D& scale);

  Vector3D transform(const Vector3D& point) const;
  void transform(const Vector3D& point, Vector3D* transformedPoint) const;
  void transform(Vector3D* point) const;
  void transform(Vector3D* point, int num) const;
  void transformNormal(Vector3D* normal) const;
  Vector3D transformNormal(const Vector3D& normal) const;
};

}
