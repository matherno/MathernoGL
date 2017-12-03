#pragma once

#include <cmath>
#include <iostream>
#include <sstream>

namespace mathernogl {

class Vector2D {
public:
	float x = 0.0;
	float y = 0.0;

	Vector2D();
	Vector2D(float value);
	Vector2D(float x, float y);
	Vector2D(const Vector2D& newVector);
	void makeUniform();
	Vector2D getUniform() const;
	float magnitude() const;
	std::string toString() const;
  void set(float x, float y);

	Vector2D operator+(const Vector2D& vector) const;
	Vector2D operator-(const Vector2D& vector) const;

	void operator+=(const Vector2D& vector);
	void operator-=(const Vector2D& vector);
	void operator=(const Vector2D& vector);

	void operator*=(const int& scalar);
	void operator*=(const float& scalar);

	Vector2D operator*(const int& scalar) const;
	Vector2D operator*(const float& scalar) const;
	Vector2D operator*(const Vector2D& rhs) const;

	void operator/=(const int& scalar);
	void operator/=(const float& scalar);

	Vector2D operator/(const int& scalar) const;
	Vector2D operator/(const float& scalar) const;

	bool operator==(const Vector2D& vector) const;
	bool operator!=(const Vector2D& vector) const;

};

}
