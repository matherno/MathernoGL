#include <maths/VectorOperations.h>
#include <maths/Arithmetic.h>

namespace mathernogl {

double dotProduct(const Vector2D& vector1, const Vector2D& vector2) {
	return (vector1.x * vector2.x) + (vector1.y * vector2.y);
}

double dotProduct(const Vector3D& vector1, const Vector3D& vector2) {
	return (vector1.x * vector2.x) + (vector1.y * vector2.y) + (vector1.z * vector2.z);
}

double dotProduct(const Vector4D& vector1, const Vector4D& vector2) {
	return (vector1.x * vector2.x) + (vector1.y * vector2.y) + (vector1.z * vector2.z) + (vector1.w * vector2.w);
}

Vector3D crossProduct(const Vector3D& vector1, const Vector3D& vector2) {
	return Vector3D(vector1.y * vector2.z - vector1.z * vector2.y, vector1.z * vector2.x - vector1.x * vector2.z, vector1.x * vector2.y - vector1.y * vector2.x);
}

double angleBetween(const Vector2D& vector1, const Vector2D& vector2) {
	return radToDeg(acos(dotProduct(vector1, vector2) / (vector1.magnitude() * vector2.magnitude())));
}

double angleBetween(const Vector3D& vector1, const Vector3D& vector2) {
	return radToDeg(acos(dotProduct(vector1, vector2) / (vector1.magnitude() * vector2.magnitude())));
}

double angleBetween(const Vector4D& vector1, const Vector4D& vector2) {
	return radToDeg(acos(dotProduct(vector1, vector2) / (vector1.magnitude() * vector2.magnitude())));
}

Vector3D reflect(const Vector3D& vector, const Vector3D& normal) {
  return vector - normal*dotProduct(vector, normal)*2;
}


double ccwAngleBetween(const Vector2D& first, const Vector2D& second) {
  Vector2D f = first;
  Vector2D s = second;
  f.makeUniform();
  s.makeUniform();
  double dotProduct = f.x*s.x + f.y*s.y;
  double determinant = f.x*s.y - f.y*s.x;
  double angle = atan2(determinant, dotProduct);
  return -1.0f * (float)radToDeg(angle);
  }

Vector3D lerpVector3D(const Vector3D& first, const Vector3D& second, double factor){
  factor = clampd(factor, 0, 1);
  return (first * (1.0f-factor)) + (second * factor);
  }



  }
