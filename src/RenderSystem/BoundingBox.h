#pragma once
//
// Created by matherno on 10/09/17.
//


#include "maths/Maths.h"
#include "memory"

class BoundingBox;
typedef std::shared_ptr<BoundingBox> BoundingBoxPtr;

enum FaceID{
  faceNegY,
  faceNegX,
  faceNegZ,
  facePosX,
  facePosY,
  facePosZ,
};

class BoundingBox {
public:

private:
  mathernogl::Vector3D lowerBound;
  mathernogl::Vector3D upperBound;
  bool gotBounds = false;

public:
  BoundingBox();
  BoundingBox(const mathernogl::Vector3D& lowerBound, const mathernogl::Vector3D& upperBound);

  mathernogl::Vector3D getLowerBound() const { return lowerBound; }
  mathernogl::Vector3D getUpperBound() const { return upperBound; }
  void getSize(double* x, double* y, double* z) const;
  mathernogl::Vector3D getSize() const;
  void setBounds(const mathernogl::Vector3D& lowerBound, const mathernogl::Vector3D& upperBound);
  void expandBounds(float expansion = 1e-5);

  //  will extend the bounds to fit the given point or bounding box
  void addPoint(double x, double y, double z);
  void addPoint(const mathernogl::Vector3D& point);
  void addBoundingBox(const BoundingBoxPtr geometry);

  mathernogl::Vector3D getFaceNormal(FaceID faceID) const;
  bool hitTest(const Ray* ray, double* hitTValue, FaceID* hitFaceID = nullptr) const;
  bool hitTest(const Ray* ray) const;
  bool isPointWithinBounds(const mathernogl::Vector3D& point) const;

  void getCorners(std::vector<mathernogl::Vector3D>* corners) const;
};
