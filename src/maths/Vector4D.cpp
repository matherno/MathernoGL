#include <mathernogl/maths/Vector4D.h>

namespace mathernogl {

Vector4D::Vector4D() {

}

Vector4D::Vector4D(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector4D::Vector4D(float value) {
	x = value;
	y = value;
	z = value;
	w = value;
}

Vector4D::Vector4D(const Vector4D& newVector) {
	x = newVector.x;
	y = newVector.y;
	z = newVector.z;
	w = newVector.w;
}

Vector4D::Vector4D(const Vector3D& newVector, float w) {
	x = newVector.x;
	y = newVector.y;
	z = newVector.z;
	this->w = w;
}

Vector4D::Vector4D(const Vector2D& newVector, float z, float w) {
	x = newVector.x;
	y = newVector.y;
	this->z = z;
	this->w = w;
}

void Vector4D::makeUniform() {
	float mag = magnitude();

	if (mag != 0.0) {
		x /= mag;
		y /= mag;
		z /= mag;
		w /= mag;
	}
}

Vector4D Vector4D::getUniform() const {
	Vector4D normalised(*this);
	normalised.makeUniform();
	return normalised;
}

float Vector4D::magnitude() const {
	return sqrt(x * x + y * y + z * z + w * w);
}

std::string Vector4D::toString() const {
	std::stringstream s;
	s << "x: " << x << ", y: " << y << ", z: " << z << ", w: " << w;
	return s.str();
}

void Vector4D::set(float x, float y, float z, float w) {
  this->x = x;
  this->y = y;
  this->z = z;
  this->w = w;
}

Vector4D Vector4D::operator+(const Vector4D& vector) const {
	return Vector4D(x + vector.x, y + vector.y, z + vector.z, w + vector.w);
}

Vector4D Vector4D::operator-(const Vector4D& vector) const {
	return Vector4D(x - vector.x, y - vector.y, z - vector.z, w - vector.w);
}

void Vector4D::operator=(const Vector4D& vector) {
	x = vector.x;
	y = vector.y;
	z = vector.z;
	w = vector.w;
}

void Vector4D::operator+=(const Vector4D& vector) {
	x += vector.x;
	y += vector.y;
	z += vector.z;
	w += vector.w;
}

void Vector4D::operator-=(const Vector4D& vector) {
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;
	w -= vector.w;
}

void Vector4D::operator*=(const int& scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
}

void Vector4D::operator*=(const float& scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
}

Vector4D Vector4D::operator*(const int& scalar) const {
	return Vector4D(x * scalar, y * scalar, z * scalar, w * scalar);
}

Vector4D Vector4D::operator*(const float& scalar) const {
	return Vector4D(x * scalar, y * scalar, z * scalar, w * scalar);
}

void Vector4D::operator/=(const int& scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;
}

void Vector4D::operator/=(const float& scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;
}

Vector4D Vector4D::operator/(const int& scalar) const {
	return Vector4D(x / scalar, y / scalar, z / scalar, w / scalar);
}

Vector4D Vector4D::operator/(const float& scalar) const {
	return Vector4D(x / scalar, y / scalar, z / scalar, w / scalar);
}

bool Vector4D::operator==(const Vector4D& vector) const {
	return x == vector.x && y == vector.y && z == vector.z && w == vector.w;
}

bool Vector4D::operator!=(const Vector4D& vector) const {
	return !(*this == vector);
}

Vector4D Vector4D::operator*(const Matrix4& matrix) const {
	Vector4D result;

	result.x = x * matrix.getAt(0, 0) + y * matrix.getAt(1, 0)
			+ z * matrix.getAt(2, 0) + w * matrix.getAt(3, 0);

	result.y = x * matrix.getAt(0, 1) + y * matrix.getAt(1, 1)
			+ z * matrix.getAt(2, 1) + w * matrix.getAt(3, 1);

	result.z = x * matrix.getAt(0, 2) + y * matrix.getAt(1, 2)
			+ z * matrix.getAt(2, 2) + w * matrix.getAt(3, 2);

	result.w = x * matrix.getAt(0, 3) + y * matrix.getAt(1, 3)
			+ z * matrix.getAt(2, 3) + w * matrix.getAt(3, 3);

	return result;
}

void Vector4D::operator*=(const Matrix4& matrix) {
	Vector4D result;

	result.x = x * matrix.getAt(0, 0) + y * matrix.getAt(1, 0)
			+ z * matrix.getAt(2, 0) + w * matrix.getAt(3, 0);

	result.y = x * matrix.getAt(0, 1) + y * matrix.getAt(1, 1)
			+ z * matrix.getAt(2, 1) + w * matrix.getAt(3, 1);

	result.z = x * matrix.getAt(0, 2) + y * matrix.getAt(1, 2)
			+ z * matrix.getAt(2, 2) + w * matrix.getAt(3, 2);

	result.w = x * matrix.getAt(0, 3) + y * matrix.getAt(1, 3)
			+ z * matrix.getAt(2, 3) + w * matrix.getAt(3, 3);

	*this = result;
}

Vector4D Vector4D::operator*(const Vector4D& rhs) const {
  return Vector4D(x*rhs.x, y*rhs.y, z*rhs.z, w*rhs.w);
}

}
