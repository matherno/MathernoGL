#pragma once
//
// Created by matherno on 31/07/17.
//

#include "Vector3D.h"

struct Ray {

  static Ray create(const mathernogl::Vector3D& origin, const mathernogl::Vector3D& direction){
    Ray ray;
    ray.origin = origin;
    ray.direction = direction;
    return ray;
  }

  static Ray createFromPoints(const mathernogl::Vector3D& point1, const mathernogl::Vector3D& point2){
    Ray ray;
    ray.origin = point1;
    ray.direction = point2 - point1;
    ray.direction.makeUniform();
    return ray;
  }

  mathernogl::Vector3D origin;
  mathernogl::Vector3D direction;
  int depth = 0;

  mathernogl::Vector3D getPosition(double t) const { return origin + direction * t; }

  static double projectPointOntoRay(const Ray* ray, mathernogl::Vector3D point){
    mathernogl::Vector3D rayOriginToPoint = point - ray->origin;
    double cosAngle = mathernogl::dotProduct(rayOriginToPoint.getUniform(), ray->direction);
    return cosAngle * rayOriginToPoint.magnitude();
  }
};
