#include <maths/Vector2D.h>

namespace mathernogl {

Vector2D::Vector2D() {

}

Vector2D::Vector2D(float x, float y) {
	this->x = x;
	this->y = y;
}

Vector2D::Vector2D(float value) {
	x = value;
	y = value;
}

Vector2D::Vector2D(const Vector2D& newVector) {
	x = newVector.x;
	y = newVector.y;
}

void Vector2D::makeUniform() {
	float mag = magnitude();

	if (mag != 0.0) {
		x /= mag;
		y /= mag;
	}
}

Vector2D Vector2D::getUniform() const {
	Vector2D normalised(*this);
	normalised.makeUniform();
	return normalised;
}

float Vector2D::magnitude() const {
	return sqrt(x * x + y * y);
}

std::string Vector2D::toString() const {
	std::ostringstream s;
	s << "x: " << x << ", y: " << y;
	return s.str();
}

void Vector2D::set(float x, float y) {
  this->x = x;
  this->y = y;
}

Vector2D Vector2D::operator+(const Vector2D& vector) const {
	return Vector2D(x + vector.x, y + vector.y);
}

Vector2D Vector2D::operator-(const Vector2D& vector) const {
	return Vector2D(x - vector.x, y - vector.y);
}

void Vector2D::operator=(const Vector2D& vector) {
	x = vector.x;
	y = vector.y;
}

void Vector2D::operator+=(const Vector2D& vector) {
	x += vector.x;
	y += vector.y;
}

void Vector2D::operator-=(const Vector2D& vector) {
	x -= vector.x;
	y -= vector.y;
}

void Vector2D::operator*=(const int& scalar) {
	x *= scalar;
	y *= scalar;
}
void Vector2D::operator*=(const float& scalar) {
	x *= scalar;
	y *= scalar;
}

Vector2D Vector2D::operator*(const int& scalar) const {
	return Vector2D(x * scalar, y * scalar);
}

Vector2D Vector2D::operator*(const float& scalar) const {
	return Vector2D(x * scalar, y * scalar);
}

void Vector2D::operator/=(const int& scalar) {
	x /= scalar;
	y /= scalar;
}

void Vector2D::operator/=(const float& scalar) {
	x /= scalar;
	y /= scalar;
}

Vector2D Vector2D::operator/(const int& scalar) const {
	return Vector2D(x / scalar, y / scalar);
}

Vector2D Vector2D::operator/(const float& scalar) const {
	return Vector2D(x / scalar, y / scalar);
}

bool Vector2D::operator==(const Vector2D& vector) const {
	return x == vector.x && y == vector.y;
}

bool Vector2D::operator!=(const Vector2D& vector) const {
	return !(*this == vector);
}

Vector2D Vector2D::operator*(const Vector2D& rhs) const {
  return Vector2D(x*rhs.x, y*rhs.y);
}

}
