//
// Created by matherno on 1/10/17.
//

#include "mathernogl/maths/Transform.h"
#include "mathernogl/maths/MatrixOperations.h"

mathernogl::Transform::Transform()
    : transformMatrix(mathernogl::matrixIdentity()) {
}

mathernogl::Transform::Transform(const mathernogl::Transform& transform)
    : transformMatrix(transform.transformMatrix) {
}

mathernogl::Transform::Transform(const mathernogl::Matrix4& transformMatrix)
    : transformMatrix(transformMatrix) {
}

void mathernogl::Transform::setTransformMatrix(const mathernogl::Matrix4& transformMatrix) {
  this->transformMatrix = transformMatrix;
}

void mathernogl::Transform::setIdentityMatrix() {
  transformMatrix = mathernogl::matrixIdentity();
}

void mathernogl::Transform::translate(float x, float y, float z) {
  transformMatrix *= mathernogl::matrixTranslate(x, y, z);
}

void mathernogl::Transform::translate(const mathernogl::Vector3D& translation) {
  transformMatrix *= mathernogl::matrixTranslate(translation);
}

void mathernogl::Transform::rotate(float axisX, float axisY, float axisZ, float angle) {
  transformMatrix *= mathernogl::matrixRotate(axisX, axisY, axisZ, angle);
}

void mathernogl::Transform::rotate(const mathernogl::Vector3D& axis, float angle) {
  transformMatrix *= mathernogl::matrixRotate(axis, angle);
}

void mathernogl::Transform::rotateBetween(const mathernogl::Vector3D& from, const mathernogl::Vector3D& to){
  transformMatrix *= mathernogl::matrixRotateBetween(from, to);
}

void mathernogl::Transform::scale(float scale) {
  transformMatrix *= mathernogl::matrixScale(scale, scale, scale);
}

void mathernogl::Transform::scale(float scaleX, float scaleY, float scaleZ) {
  transformMatrix *= mathernogl::matrixScale(scaleX, scaleY, scaleZ);
}

void mathernogl::Transform::scale(const mathernogl::Vector3D& scale) {
  transformMatrix *= mathernogl::matrixScale(scale);
}

mathernogl::Vector3D mathernogl::Transform::transform(const mathernogl::Vector3D& point) const {
  return point * transformMatrix;
}

void mathernogl::Transform::transform(const mathernogl::Vector3D& point, mathernogl::Vector3D* transformedPoint) const {
  *transformedPoint = point * transformMatrix;
}

void mathernogl::Transform::transform(mathernogl::Vector3D* point) const {
  *point *= transformMatrix;
}

void mathernogl::Transform::transform(mathernogl::Vector3D* point, int num) const {
  for (int pointNum = 0; pointNum < num; ++pointNum)
    point[pointNum] *= transformMatrix;
}

bool mathernogl::Transform::isIdentity() const {
  return transformMatrix == mathernogl::matrixIdentity();
}

void mathernogl::Transform::invert() {
  transformMatrix = matrixInverse(transformMatrix);
}

mathernogl::Transform mathernogl::Transform::getInverse() const {
  return matrixInverse(transformMatrix);
}

void mathernogl::Transform::transformNormal(mathernogl::Vector3D* normal) const {
  *normal = transformNormal(*normal);
}

mathernogl::Vector3D mathernogl::Transform::transformNormal(const mathernogl::Vector3D& normal) const {
  mathernogl::Matrix4 transformClone = transformMatrix;
  transformClone.setAt(0, 3, 0);
  transformClone.setAt(0, 3, 1);
  transformClone.setAt(0, 3, 2);
  return (normal * transformClone).getUniform();
}

mathernogl::Vector3D mathernogl::Transform::getTranslationComponent() const {
  float divisor = transformMatrix.getAt(3, 3);
  if (divisor == 0) {
    return Vector3D(0);
  }
  else {
    divisor = 1.0f / divisor;
    return mathernogl::Vector3D(transformMatrix.getAt(3, 0)*divisor, transformMatrix.getAt(3, 1)*divisor, transformMatrix.getAt(3, 2)*divisor);
  }
}


