#pragma once
//
// Created by matt on 5/06/17.
//


namespace mathernogl{

const double pi = 3.14159265358979323846;

double radToDeg(double radians);
double degToRad(double degrees);
bool compareFloat(float a, float b, unsigned int maxUlps = 1);
float clampf(float value, float min, float max);
int clampi(int value, int min, int max);

}
