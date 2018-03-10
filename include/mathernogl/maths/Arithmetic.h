#pragma once
//
// Created by matt on 5/06/17.
//


#include "Vector2D.h"
#include "Vector3D.h"

namespace mathernogl{

const double pi = 3.14159265358979323846;

double radToDeg(double radians);
double degToRad(double degrees);
bool compareFloat(float a, float b, unsigned int maxUlps = 1);
double clampd(double value, double min, double max);
float clampf(float value, float min, float max);
int clampi(int value, int min, int max);
double ccwAngleBetween(const Vector2D& first, const Vector2D& second);
Vector3D lerpVector3D(const Vector3D& first, const Vector3D& second, double factor);

}
