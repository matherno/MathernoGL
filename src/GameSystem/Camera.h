#pragma once

#include <maths/Transform.h>

class Camera
  {
private:
  bool valid = false;
  mathernogl::Transform worldToCamera;
  mathernogl::Transform cameraToClip;

public:
  Camera() {};
  virtual ~Camera() {};

  mathernogl::Transform* getWorldToCamera() { return &worldToCamera; }
  mathernogl::Transform* getCameraToClip() { return &cameraToClip; }
  //todo: do something better here? cache the camera pos?
  Vector3D getCameraPos() const { return worldToCamera.getInverse().transform(Vector3D(0, 0, 0)); }
  void setValid(bool valid){ this->valid = valid; }
  bool isValid() const { return valid; }
  };
