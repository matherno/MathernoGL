#include <mathernogl/maths/Vector3D.h>

namespace mathernogl {

Vector3D::Vector3D() {

}

Vector3D::Vector3D(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3D::Vector3D(float value) {
	x = value;
	y = value;
	z = value;
}

Vector3D::Vector3D(const Vector4D& newVector) {
	x = newVector.x;
	y = newVector.y;
	z = newVector.z;
}

Vector3D::Vector3D(const Vector3D& newVector) {
	x = newVector.x;
	y = newVector.y;
	z = newVector.z;
}

Vector3D::Vector3D(const Vector2D& newVector, float z) {
	x = newVector.x;
	y = newVector.y;
	this->z = z;
}

void Vector3D::makeUniform() {
	float mag = magnitude();

	if (mag != 0.0) {
		x /= mag;
		y /= mag;
		z /= mag;
	}
}

Vector3D Vector3D::getUniform() const {
	Vector3D normalised(*this);
	normalised.makeUniform();
	return normalised;
}

float Vector3D::magnitude() const {
	return sqrt(x * x + y * y + z * z);
}

std::string Vector3D::toString() const {
	std::ostringstream s;
	s << "x: " << x << ", y: " << y << ", z: " << z;
	return s.str();
}

void Vector3D::set(float x, float y, float z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

float Vector3D::distanceToPoint(const Vector3D& point) const {
  return (*this - point).magnitude();
}

Vector3D Vector3D::operator+(const Vector3D& vector) const {
	return Vector3D(x + vector.x, y + vector.y, z + vector.z);
}

Vector3D Vector3D::operator-(const Vector3D& vector) const {
	return Vector3D(x - vector.x, y - vector.y, z - vector.z);
}

void Vector3D::operator=(const Vector3D& vector) {
	x = vector.x;
	y = vector.y;
	z = vector.z;
}

void Vector3D::operator+=(const Vector3D& vector) {
	x += vector.x;
	y += vector.y;
	z += vector.z;
}

void Vector3D::operator-=(const Vector3D& vector) {
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;
}

void Vector3D::operator*=(const int& scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
}

void Vector3D::operator*=(const float& scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
}

Vector3D Vector3D::operator*(const int& scalar) const {
	return Vector3D(x * scalar, y * scalar, z * scalar);
}

Vector3D Vector3D::operator*(const float& scalar) const {
	return Vector3D(x * scalar, y * scalar, z * scalar);
}

Vector3D Vector3D::operator*(const Vector3D& rhs) const {
  return Vector3D(x*rhs.x, y*rhs.y, z*rhs.z);
}

void Vector3D::operator/=(const int& scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;
}


void Vector3D::operator/=(const float& scalar) {
	x /= scalar;
	y /= scalar;
	z /= scalar;
}

Vector3D Vector3D::operator/(const int& scalar) const {
	return Vector3D(x / scalar, y / scalar, z / scalar);
}


Vector3D Vector3D::operator/(const float& scalar) const {
	return Vector3D(x / scalar, y / scalar, z / scalar);
}

bool Vector3D::operator==(const Vector3D& vector) const {
	return x == vector.x && y == vector.y && z == vector.z;
}

bool Vector3D::operator!=(const Vector3D& vector) const {
	return !(*this == vector);
}

Vector3D Vector3D::operator*(const Matrix4& matrix) const {
	Vector3D result;

	result.x = x * matrix.getAt(0, 0) + y * matrix.getAt(1, 0) + z * matrix.getAt(2, 0) + 1 * matrix.getAt(3, 0);

	result.y = x * matrix.getAt(0, 1) + y * matrix.getAt(1, 1) + z * matrix.getAt(2, 1) + 1 * matrix.getAt(3, 1);

	result.z = x * matrix.getAt(0, 2) + y * matrix.getAt(1, 2) + z * matrix.getAt(2, 2) + 1 * matrix.getAt(3, 2);

	float w = x * matrix.getAt(0, 3) + y * matrix.getAt(1, 3) + z * matrix.getAt(2, 3) + 1 * matrix.getAt(3, 3);

	return result / w;
}

void Vector3D::operator*=(const Matrix4& matrix) {
	Vector3D result;

	result.x = x * matrix.getAt(0, 0) + y * matrix.getAt(1, 0) + z * matrix.getAt(2, 0) + 1 * matrix.getAt(3, 0);

	result.y = x * matrix.getAt(0, 1) + y * matrix.getAt(1, 1) + z * matrix.getAt(2, 1) + 1 * matrix.getAt(3, 1);

	result.z = x * matrix.getAt(0, 2) + y * matrix.getAt(1, 2) + z * matrix.getAt(2, 2) + 1 * matrix.getAt(3, 2);

	float w = x * matrix.getAt(0, 3) + y * matrix.getAt(1, 3) + z * matrix.getAt(2, 3) + 1 * matrix.getAt(3, 3);

	*this = result / w;
}



}
