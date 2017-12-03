#pragma once
//
// Created by matt on 23/04/17.
//

#include <mathernogl/maths/Vector3D.h>

namespace mathernogl {

class CameraController {
private:
    Vector3D position;
    Vector3D rotation;      //eulor rotation

public:
    CameraController() {};
    CameraController(const Vector3D& position, const Vector3D& rotation)
            : position(position), rotation(rotation) {}

    Vector3D getPosition() const;
    void setPosition(const Vector3D& position);
    void translate(const Vector3D& translation);
    Vector3D getRotation() const;
    Matrix4 getMatrixRotation() const;
    void setRotation(const Vector3D& rotation);
    void rotate(const Vector3D& axis, float angle);
    void pitch(float angle);
    void yaw(float angle);
    void roll(float angle);

};

}