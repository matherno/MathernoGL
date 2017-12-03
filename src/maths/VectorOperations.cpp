#include <mathernogl/maths/VectorOperations.h>
#include <mathernogl/maths/Arithmetic.h>

namespace mathernogl {

float dotProduct(const Vector2D& vector1, const Vector2D& vector2) {
	return (vector1.x * vector2.x) + (vector1.y * vector2.y);
}

float dotProduct(const Vector3D& vector1, const Vector3D& vector2) {
	return (vector1.x * vector2.x) + (vector1.y * vector2.y) + (vector1.z * vector2.z);
}

float dotProduct(const Vector4D& vector1, const Vector4D& vector2) {
	return (vector1.x * vector2.x) + (vector1.y * vector2.y) + (vector1.z * vector2.z) + (vector1.w * vector2.w);
}

Vector3D crossProduct(const Vector3D& vector1, const Vector3D& vector2) {
	return Vector3D(vector1.y * vector2.z - vector1.z * vector2.y, vector1.z * vector2.x - vector1.x * vector2.z, vector1.x * vector2.y - vector1.y * vector2.x);
}

//only uses the x, y, and z components for cross product. assumes vectors are homogenous (w = 1)
Vector4D crossProduct(const Vector4D& vector1, const Vector4D& vector2) {
	Vector4D result = Vector4D(crossProduct(Vector3D(vector1), Vector3D(vector2)));
	result.w = 1.0;
	return result;
}

float angleBetween(const Vector2D& vector1, const Vector2D& vector2) {
	return radToDeg(acos(dotProduct(vector1, vector2) / (vector1.magnitude() * vector2.magnitude())));
}

float angleBetween(const Vector3D& vector1, const Vector3D& vector2) {
	return radToDeg(acos(dotProduct(vector1, vector2) / (vector1.magnitude() * vector2.magnitude())));
}

float angleBetween(const Vector4D& vector1, const Vector4D& vector2) {
	return radToDeg(acos(dotProduct(vector1, vector2) / (vector1.magnitude() * vector2.magnitude())));
}

mathernogl::Vector3D reflect(const mathernogl::Vector3D& vector, const mathernogl::Vector3D& normal) {
  return vector - normal*mathernogl::dotProduct(vector, normal)*2;
}


}
