//
// Created by matt on 23/04/17.
//

#include <mathernogl/maths/Maths.h>
#include "mathernogl/world/CameraController.h"

namespace mathernogl{

Vector3D CameraController::getPosition() const {
    return position;
}

void CameraController::setPosition(const Vector3D& position) {
    this->position = position;
}

void CameraController::translate(const Vector3D &translation) {
    this->position += translation;
}

Vector3D CameraController::getRotation() const {
    return rotation;
}

Matrix4 CameraController::getMatrixRotation() const {
    return matrixRoll(rotation.z) * matrixPitch(rotation.x) * matrixYaw(rotation.y) ;
}

void CameraController::setRotation(const Vector3D& rotation) {
    this->rotation = rotation;
}

void CameraController::rotate(const Vector3D &axis, float angle) {
    this->rotation *= matrixRotate(axis, angle);
}

void CameraController::pitch(float angle) {
    this->rotation.x += angle;
}

void CameraController::yaw(float angle) {
    this->rotation.y += angle;
}

void CameraController::roll(float angle) {
    this->rotation.z += angle;
}


}