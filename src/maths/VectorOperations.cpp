#include <mathernogl/maths/VectorOperations.h>
#include <mathernogl/maths/Arithmetic.h>

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


}
