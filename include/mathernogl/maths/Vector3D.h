#pragma once

#include <cmath>
#include <iostream>
#include <sstream>

#include <mathernogl/maths/Vector2D.h>
#include <mathernogl/maths/Vector4D.h>
#include <mathernogl/maths/Matrix4.h>

namespace mathernogl {

class Vector4D;

class Vector3D {
public:
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;

	Vector3D();
	Vector3D(float x, float y, float z);
	Vector3D(float value);
	Vector3D(const Vector4D& newVector);
	Vector3D(const Vector3D& newVector);
	Vector3D(const Vector2D& newVector, float z = 0);
	void makeUniform();
	Vector3D getUniform() const;
	float magnitude() const;
	std::string toString() const;
  void set(float x, float y, float z);
  float distanceToPoint(const Vector3D& point) const;
  Vector3D inverse() const;

	Vector3D operator+(const Vector3D& vector) const;
	Vector3D operator-(const Vector3D& vector) const;

	void operator=(const Vector3D& vector);
	void operator+=(const Vector3D& vector);
	void operator-=(const Vector3D& vector);

	void operator*=(const int& scalar);
	void operator*=(const float& scalar);

	Vector3D operator*(const int& scalar) const;
	Vector3D operator*(const float& scalar) const;
	Vector3D operator*(const Vector3D& rhs) const;

	void operator/=(const int& scalar);
	void operator/=(const float& scalar);

	Vector3D operator/(const int& scalar) const;
	Vector3D operator/(const float& scalar) const;

	bool operator==(const Vector3D& vector) const;
	bool operator!=(const Vector3D& vector) const;

	Vector3D operator*(const Matrix4& matrix) const;
	void operator*=(const Matrix4& matrix);

};

}
