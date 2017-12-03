#pragma once

#include <cmath>
#include <iostream>
#include <sstream>

#include <mathernogl/maths/Vector2D.h>
#include <mathernogl/maths/Vector3D.h>
#include <mathernogl/maths/Matrix4.h>

namespace mathernogl {

class Vector3D;

class Vector4D {
public:
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
	float w = 0.0;

	Vector4D();
	Vector4D(float x, float y, float z, float w);
	Vector4D(float value);
	Vector4D(const Vector4D& newVector);
	Vector4D(const Vector3D& newVector, float w = 1);
	Vector4D(const Vector2D& newVector, float z = 0, float w = 1);
  void set(float x, float y, float z, float w);
	void makeUniform();
	Vector4D getUniform() const;
	float magnitude() const;
	std::string toString() const;

	Vector4D operator+(const Vector4D& vector) const;
	Vector4D operator-(const Vector4D& vector) const;

	void operator=(const Vector4D& vector);
	void operator+=(const Vector4D& vector);
	void operator-=(const Vector4D& vector);

	void operator*=(const int& scalar);
	void operator*=(const float& scalar);

	Vector4D operator*(const int& scalar) const;
	Vector4D operator*(const float& scalar) const;
	Vector4D operator*(const Vector4D& rhs) const;

	void operator/=(const int& scalar);
	void operator/=(const float& scalar);

	Vector4D operator/(const int& scalar) const;
	Vector4D operator/(const float& scalar) const;

	bool operator==(const Vector4D& vector) const;
	bool operator!=(const Vector4D& vector) const;

	Vector4D operator*(const Matrix4& matrix) const;
	void operator*=(const Matrix4& matrix);

};

}
