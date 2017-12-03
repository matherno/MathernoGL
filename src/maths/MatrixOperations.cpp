#include <mathernogl/maths/MatrixOperations.h>
#include <mathernogl/maths/Arithmetic.h>

namespace mathernogl {

Matrix4 matrixIdentity() {
	return Matrix4(1);
}

Matrix4 matrixTranslate(const Vector3D& translation) {
	std::vector<float> matrix = { 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, translation.x, translation.y, translation.z, 1.0 };

	return Matrix4(matrix);
}

Matrix4 matrixTranslate(const float x, const float y, const float z) {
	return matrixTranslate(Vector3D(x, y, z));
}

Matrix4 matrixRotate(const Vector3D& rotationAxis, const float rotationAngle) {
	const float cosAngle = cos(degToRad(-rotationAngle));
	const float sinAngle = sin(degToRad(-rotationAngle));
	const float x = rotationAxis.x;
	const float y = rotationAxis.y;
	const float z = rotationAxis.z;

	std::vector<float> matrix(16);
	matrix[0] = cosAngle + x * x * (1 - cosAngle);
	matrix[1] = x * y * (1 - cosAngle) - z * sinAngle;
	matrix[2] = x * z * (1 - cosAngle) + y * sinAngle;
	matrix[3] = 0.0f;
	matrix[4] = y * x * (1 - cosAngle) + z * sinAngle;
	matrix[5] = cosAngle + y * y * (1 - cosAngle);
	matrix[6] = y * z * (1 - cosAngle) - x * sinAngle;
	matrix[7] = 0.0f;
	matrix[8] = z * x * (1 - cosAngle) - y * sinAngle;
	matrix[9] = z * y * (1 - cosAngle) + x * sinAngle;
	matrix[10] = cosAngle + z * z * (1 - cosAngle);
	matrix[11] = 0.0f;
	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = 0.0f;
	matrix[15] = 1.0f;

	return Matrix4(matrix);
}

Matrix4 matrixRotate(const float x, const float y, const float z, const float rotationAngle) {
	return matrixRotate(Vector3D(x, y, z), rotationAngle);
}

Matrix4 matrixPitch(float angle) {
    const float cosAngle = cos(degToRad(angle));
    const float sinAngle = sin(degToRad(angle));
    std::vector<float> matrix(16);
    matrix[0] = 1.0f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = cosAngle;
    matrix[6] = sinAngle;
    matrix[7] = 0.0f;
    matrix[8] = 0.0f;
    matrix[9] = -sinAngle;
    matrix[10] = cosAngle;
    matrix[11] = 0.0f;
    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
    return Matrix4(matrix);
}

Matrix4 matrixYaw(float angle) {
    const float cosAngle = cos(degToRad(angle));
    const float sinAngle = sin(degToRad(angle));
    std::vector<float> matrix(16);
    matrix[0] = cosAngle;
    matrix[1] = 0.0f;
    matrix[2] = -sinAngle;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = 1.0f;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;
    matrix[8] = sinAngle;
    matrix[9] = 0;
    matrix[10] = cosAngle;
    matrix[11] = 0.0f;
    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
    return Matrix4(matrix);
}

Matrix4 matrixRoll(float angle) {
    const float cosAngle = cos(degToRad(angle));
    const float sinAngle = sin(degToRad(angle));
    std::vector<float> matrix(16);
    matrix[0] = cosAngle;
    matrix[1] = sinAngle;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;
    matrix[4] = -sinAngle;
    matrix[5] = cosAngle;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;
    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = 1.0f;
    matrix[11] = 0.0f;
    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
    return Matrix4(matrix);
}

Matrix4 matrixScale(const Vector3D& scaling) {
	std::vector<float> matrix = { scaling.x, 0.0, 0.0, 0.0,
									0.0, scaling.y, 0.0, 0.0,
									0.0, 0.0, scaling.z, 0.0,
									0.0, 0.0, 0.0, 1.0 };

	return Matrix4(matrix);
}

Matrix4 matrixScale(const float x, const float y, const float z) {
	return matrixScale(Vector3D(x, y, z));
}

Matrix4 matrixScale(const float scaling) {
	return matrixScale(Vector3D(scaling, scaling, scaling));
}

Matrix4 matrixPerspective(const float fov, const float aspectRatio, float zNearPlane, float zFarPlane) {
	std::vector<float> matrix(16);
	zNearPlane *= -1;
	zFarPlane *= -1;
	for (int index = 0; index < 16; index++) {
		matrix[index] = 0;
	}
	matrix[0] = 2.0 / fov;
	matrix[5] = 2.0 * aspectRatio / fov;
	matrix[10] = (zFarPlane + zNearPlane) / (zNearPlane - zFarPlane);
	matrix[14] = (2.0 * zFarPlane * zNearPlane) / (zNearPlane - zFarPlane);
	matrix[11] = -1.0;

	return Matrix4(matrix);
}

Matrix4 matrixOrthogonal(const float fov, const float aspectRatio, const float zFarPlane) {
	std::vector<float> matrix(16);
	for (int index = 0; index < 16; index++) {
		matrix[index] = 0;
	}
	matrix[0] = 2.0 / fov;
	matrix[5] = 2.0 * aspectRatio / fov;
	matrix[10] = 2.0 / zFarPlane;
	matrix[14] = -1.0;
	matrix[15] = 1.0;

	return Matrix4(matrix);
}

Matrix4 matrixInverse(const Matrix4& matrix) {
	std::vector<float> inverse(16);
	const std::vector<float> matrixArray = matrix.getArray();

	inverse[0] = matrixArray[5] * matrixArray[10] * matrixArray[15] - matrixArray[5] * matrixArray[11] * matrixArray[14] - matrixArray[9] * matrixArray[6] * matrixArray[15] + matrixArray[9] * matrixArray[7] * matrixArray[14] + matrixArray[13] * matrixArray[6] * matrixArray[11] - matrixArray[13] * matrixArray[7] * matrixArray[10];
	inverse[4] = -matrixArray[4] * matrixArray[10] * matrixArray[15] + matrixArray[4] * matrixArray[11] * matrixArray[14] + matrixArray[8] * matrixArray[6] * matrixArray[15] - matrixArray[8] * matrixArray[7] * matrixArray[14] - matrixArray[12] * matrixArray[6] * matrixArray[11] + matrixArray[12] * matrixArray[7] * matrixArray[10];
	inverse[8] = matrixArray[4] * matrixArray[9] * matrixArray[15] - matrixArray[4] * matrixArray[11] * matrixArray[13] - matrixArray[8] * matrixArray[5] * matrixArray[15] + matrixArray[8] * matrixArray[7] * matrixArray[13] + matrixArray[12] * matrixArray[5] * matrixArray[11] - matrixArray[12] * matrixArray[7] * matrixArray[9];
	inverse[12] = -matrixArray[4] * matrixArray[9] * matrixArray[14] + matrixArray[4] * matrixArray[10] * matrixArray[13] + matrixArray[8] * matrixArray[5] * matrixArray[14] - matrixArray[8] * matrixArray[6] * matrixArray[13] - matrixArray[12] * matrixArray[5] * matrixArray[10] + matrixArray[12] * matrixArray[6] * matrixArray[9];
	inverse[1] = -matrixArray[1] * matrixArray[10] * matrixArray[15] + matrixArray[1] * matrixArray[11] * matrixArray[14] + matrixArray[9] * matrixArray[2] * matrixArray[15] - matrixArray[9] * matrixArray[3] * matrixArray[14] - matrixArray[13] * matrixArray[2] * matrixArray[11] + matrixArray[13] * matrixArray[3] * matrixArray[10];
	inverse[5] = matrixArray[0] * matrixArray[10] * matrixArray[15] - matrixArray[0] * matrixArray[11] * matrixArray[14] - matrixArray[8] * matrixArray[2] * matrixArray[15] + matrixArray[8] * matrixArray[3] * matrixArray[14] + matrixArray[12] * matrixArray[2] * matrixArray[11] - matrixArray[12] * matrixArray[3] * matrixArray[10];
	inverse[9] = -matrixArray[0] * matrixArray[9] * matrixArray[15] + matrixArray[0] * matrixArray[11] * matrixArray[13] + matrixArray[8] * matrixArray[1] * matrixArray[15] - matrixArray[8] * matrixArray[3] * matrixArray[13] - matrixArray[12] * matrixArray[1] * matrixArray[11] + matrixArray[12] * matrixArray[3] * matrixArray[9];
	inverse[13] = matrixArray[0] * matrixArray[9] * matrixArray[14] - matrixArray[0] * matrixArray[10] * matrixArray[13] - matrixArray[8] * matrixArray[1] * matrixArray[14] + matrixArray[8] * matrixArray[2] * matrixArray[13] + matrixArray[12] * matrixArray[1] * matrixArray[10] - matrixArray[12] * matrixArray[2] * matrixArray[9];
	inverse[2] = matrixArray[1] * matrixArray[6] * matrixArray[15] - matrixArray[1] * matrixArray[7] * matrixArray[14] - matrixArray[5] * matrixArray[2] * matrixArray[15] + matrixArray[5] * matrixArray[3] * matrixArray[14] + matrixArray[13] * matrixArray[2] * matrixArray[7] - matrixArray[13] * matrixArray[3] * matrixArray[6];
	inverse[6] = -matrixArray[0] * matrixArray[6] * matrixArray[15] + matrixArray[0] * matrixArray[7] * matrixArray[14] + matrixArray[4] * matrixArray[2] * matrixArray[15] - matrixArray[4] * matrixArray[3] * matrixArray[14] - matrixArray[12] * matrixArray[2] * matrixArray[7] + matrixArray[12] * matrixArray[3] * matrixArray[6];
	inverse[10] = matrixArray[0] * matrixArray[5] * matrixArray[15] - matrixArray[0] * matrixArray[7] * matrixArray[13] - matrixArray[4] * matrixArray[1] * matrixArray[15] + matrixArray[4] * matrixArray[3] * matrixArray[13] + matrixArray[12] * matrixArray[1] * matrixArray[7] - matrixArray[12] * matrixArray[3] * matrixArray[5];
	inverse[14] = -matrixArray[0] * matrixArray[5] * matrixArray[14] + matrixArray[0] * matrixArray[6] * matrixArray[13] + matrixArray[4] * matrixArray[1] * matrixArray[14] - matrixArray[4] * matrixArray[2] * matrixArray[13] - matrixArray[12] * matrixArray[1] * matrixArray[6] + matrixArray[12] * matrixArray[2] * matrixArray[5];
	inverse[3] = -matrixArray[1] * matrixArray[6] * matrixArray[11] + matrixArray[1] * matrixArray[7] * matrixArray[10] + matrixArray[5] * matrixArray[2] * matrixArray[11] - matrixArray[5] * matrixArray[3] * matrixArray[10] - matrixArray[9] * matrixArray[2] * matrixArray[7] + matrixArray[9] * matrixArray[3] * matrixArray[6];
	inverse[7] = matrixArray[0] * matrixArray[6] * matrixArray[11] - matrixArray[0] * matrixArray[7] * matrixArray[10] - matrixArray[4] * matrixArray[2] * matrixArray[11] + matrixArray[4] * matrixArray[3] * matrixArray[10] + matrixArray[8] * matrixArray[2] * matrixArray[7] - matrixArray[8] * matrixArray[3] * matrixArray[6];
	inverse[11] = -matrixArray[0] * matrixArray[5] * matrixArray[11] + matrixArray[0] * matrixArray[7] * matrixArray[9] + matrixArray[4] * matrixArray[1] * matrixArray[11] - matrixArray[4] * matrixArray[3] * matrixArray[9] - matrixArray[8] * matrixArray[1] * matrixArray[7] + matrixArray[8] * matrixArray[3] * matrixArray[5];
	inverse[15] = matrixArray[0] * matrixArray[5] * matrixArray[10] - matrixArray[0] * matrixArray[6] * matrixArray[9] - matrixArray[4] * matrixArray[1] * matrixArray[10] + matrixArray[4] * matrixArray[2] * matrixArray[9] + matrixArray[8] * matrixArray[1] * matrixArray[6] - matrixArray[8] * matrixArray[2] * matrixArray[5];

	const float determinant = matrixArray[0] * inverse[0] + matrixArray[1] * inverse[4] + matrixArray[2] * inverse[8] + matrixArray[3] * inverse[12];

	if (determinant != 0) {
		for (int i = 0; i < 16; i++) {
			inverse[i] = inverse[i] * (1.0 / determinant);
		}
	}
	else {
		for (int i = 0; i < 16; i++) {
			inverse[i] = 0;
		}
	}

	return Matrix4(inverse);
}

}
