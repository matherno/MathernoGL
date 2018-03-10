#pragma once

#include <cmath>
#include <iostream>
#include <sstream>

#include <mathernogl/maths/Vector2D.h>
#include <mathernogl/maths/Matrix4.h>
#include <mathernogl/image/ImageData.h>

namespace mathernogl {

template <class T> class TVector3D {
public:
	T x = 0.0;
	T y = 0.0;
	T z = 0.0;

	TVector3D();
	TVector3D(T x, T y, T z);
	TVector3D(T value);
	TVector3D(const TVector3D<T>& newVector);
	TVector3D(const Vector2D& newVector, T z = 0);
	void makeUniform();
	TVector3D<T> getUniform() const;
  T magnitude() const;
	std::string toString() const;
  void set(T x, T y, T z);
  T distanceToPoint(const TVector3D<T>& point) const;
  TVector3D<T> inverse() const;

	TVector3D<T> operator+(const TVector3D<T>& vector) const;
	TVector3D<T> operator-(const TVector3D<T>& vector) const;

	void operator=(const TVector3D<T>& vector);
	void operator+=(const TVector3D<T>& vector);
	void operator-=(const TVector3D<T>& vector);

	void operator*=(const int& scalar);
	void operator*=(const T& scalar);

	TVector3D<T> operator*(const int& scalar) const;
	TVector3D<T> operator*(const T& scalar) const;
	TVector3D<T> operator*(const TVector3D<T>& rhs) const;

	void operator/=(const int& scalar);
	void operator/=(const T& scalar);

	TVector3D<T> operator/(const int& scalar) const;
	TVector3D<T> operator/(const T& scalar) const;

	bool operator==(const TVector3D<T>& vector) const;
	bool operator!=(const TVector3D<T>& vector) const;

	TVector3D<T> operator*(const Matrix4& matrix) const;
	void operator*=(const Matrix4& matrix);

  static TVector3D<T> glColour(byte red, byte green, byte blue);
};


template<class T> TVector3D<T>::TVector3D() {

}

template<class T> TVector3D<T>::TVector3D(T x, T y, T z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

template<class T> TVector3D<T>::TVector3D(T value) {
  x = value;
  y = value;
  z = value;
}

template<class T> TVector3D<T>::TVector3D(const TVector3D<T>& newVector) {
  x = newVector.x;
  y = newVector.y;
  z = newVector.z;
}

template<class T> TVector3D<T>::TVector3D(const Vector2D& newVector, T z) {
  x = newVector.x;
  y = newVector.y;
  this->z = z;
}

template<class T> void TVector3D<T>::makeUniform() {
  T mag = magnitude();

  if (mag != 0.0) {
    x /= mag;
    y /= mag;
    z /= mag;
    }
}

template<class T> TVector3D<T> TVector3D<T>::getUniform() const {
  TVector3D normalised(*this);
  normalised.makeUniform();
  return normalised;
}

template<class T> T TVector3D<T>::magnitude() const {
  return sqrt(x * x + y * y + z * z);
}

template<class T> std::string TVector3D<T>::toString() const {
  std::ostringstream s;
  s << "x: " << x << ", y: " << y << ", z: " << z;
  return s.str();
}

template<class T> void TVector3D<T>::set(T x, T y, T z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

template<class T> T TVector3D<T>::distanceToPoint(const TVector3D<T>& point) const {
  return (*this - point).magnitude();
}

template<class T> TVector3D<T> TVector3D<T>::inverse() const {
  return TVector3D<T>(x * -1, y * -1, z * -1);
}

template<class T> TVector3D<T> TVector3D<T>::operator+(const TVector3D<T>& vector) const {
  return TVector3D<T>(x + vector.x, y + vector.y, z + vector.z);
}

template<class T> TVector3D<T> TVector3D<T>::operator-(const TVector3D<T>& vector) const {
  return TVector3D<T>(x - vector.x, y - vector.y, z - vector.z);
}

template<class T> void TVector3D<T>::operator=(const TVector3D<T>& vector) {
  x = vector.x;
  y = vector.y;
  z = vector.z;
}

template<class T> void TVector3D<T>::operator+=(const TVector3D<T>& vector) {
  x += vector.x;
  y += vector.y;
  z += vector.z;
}

template<class T> void TVector3D<T>::operator-=(const TVector3D<T>& vector) {
  x -= vector.x;
  y -= vector.y;
  z -= vector.z;
}

template<class T> void TVector3D<T>::operator*=(const int& scalar) {
  x *= scalar;
  y *= scalar;
  z *= scalar;
}

template<class T> void TVector3D<T>::operator*=(const T& scalar) {
  x *= scalar;
  y *= scalar;
  z *= scalar;
}

template<class T> TVector3D<T> TVector3D<T>::operator*(const int& scalar) const {
  return TVector3D<T>(x * scalar, y * scalar, z * scalar);
}

template<class T> TVector3D<T> TVector3D<T>::operator*(const T& scalar) const {
  return TVector3D<T>(x * scalar, y * scalar, z * scalar);
}

template<class T> TVector3D<T> TVector3D<T>::operator*(const TVector3D<T>& rhs) const {
  return TVector3D<T>(x*rhs.x, y*rhs.y, z*rhs.z);
}

template<class T> void TVector3D<T>::operator/=(const int& scalar) {
  x /= scalar;
  y /= scalar;
  z /= scalar;
}


template<class T> void TVector3D<T>::operator/=(const T& scalar) {
  x /= scalar;
  y /= scalar;
  z /= scalar;
}

template<class T> TVector3D<T> TVector3D<T>::operator/(const int& scalar) const {
  return TVector3D<T>(x / scalar, y / scalar, z / scalar);
}


template<class T> TVector3D<T> TVector3D<T>::operator/(const T& scalar) const {
  return TVector3D<T>(x / scalar, y / scalar, z / scalar);
}

template<class T> bool TVector3D<T>::operator==(const TVector3D<T>& vector) const {
  return x == vector.x && y == vector.y && z == vector.z;
}

template<class T> bool TVector3D<T>::operator!=(const TVector3D<T>& vector) const {
  return !(*this == vector);
}

template<class T> TVector3D<T> TVector3D<T>::operator*(const Matrix4& matrix) const {
  TVector3D<T> result;

  result.x = x * matrix.getAt(0, 0) + y * matrix.getAt(1, 0) + z * matrix.getAt(2, 0) + 1 * matrix.getAt(3, 0);

  result.y = x * matrix.getAt(0, 1) + y * matrix.getAt(1, 1) + z * matrix.getAt(2, 1) + 1 * matrix.getAt(3, 1);

  result.z = x * matrix.getAt(0, 2) + y * matrix.getAt(1, 2) + z * matrix.getAt(2, 2) + 1 * matrix.getAt(3, 2);

  T w = x * matrix.getAt(0, 3) + y * matrix.getAt(1, 3) + z * matrix.getAt(2, 3) + 1 * matrix.getAt(3, 3);

  return result / w;
}

template<class T> void TVector3D<T>::operator*=(const Matrix4& matrix) {
  TVector3D<T> result;

  result.x = x * matrix.getAt(0, 0) + y * matrix.getAt(1, 0) + z * matrix.getAt(2, 0) + 1 * matrix.getAt(3, 0);

  result.y = x * matrix.getAt(0, 1) + y * matrix.getAt(1, 1) + z * matrix.getAt(2, 1) + 1 * matrix.getAt(3, 1);

  result.z = x * matrix.getAt(0, 2) + y * matrix.getAt(1, 2) + z * matrix.getAt(2, 2) + 1 * matrix.getAt(3, 2);

  T w = x * matrix.getAt(0, 3) + y * matrix.getAt(1, 3) + z * matrix.getAt(2, 3) + 1 * matrix.getAt(3, 3);

  *this = result / w;
}

template<class T> TVector3D<T> TVector3D<T>::glColour(byte red, byte green, byte blue){
  return TVector3D<T>((T)red / 255.0f, (T)green / 255.0f, (T)blue / 255.0f);
}

typedef TVector3D<double> Vector3D;

}
