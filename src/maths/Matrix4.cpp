/*	Author: Matthew Littlemore	*/
/*	Purpose: Used to wrap a float array as a 4x4 matrix	*/

#include <mathernogl/maths/Matrix4.h>

namespace mathernogl {

Matrix4::Matrix4() {
	matrix = {
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};
}

Matrix4::Matrix4(float diagonal) {
	matrix = {
		diagonal, 0, 0, 0,
		0, diagonal, 0, 0,
		0, 0, diagonal, 0,
		0, 0, 0, diagonal
	};
}

Matrix4::Matrix4(const std::vector<float>& matrixToCopy) {
	matrix = matrixToCopy;
}

Matrix4::Matrix4(const Matrix4& matrixToCopy) {
	matrix = matrixToCopy.getArray();
}

void Matrix4::setAt(float value, int row, int col) {
	matrix[col + row * 4] = value;
}

float Matrix4::getAt(int row, int col) const {
	return matrix[col + row * 4];
}

void Matrix4::setArray(const std::vector<float>& matrixToCopy) {
	matrix = matrixToCopy;
}

const std::vector<float>& Matrix4::getArray() const {
	return matrix;
}

const float* Matrix4::getCArray() const {
	return &matrix[0];
}

std::string Matrix4::toString() const {
	std::stringstream matrixPrint;

	matrixPrint << "";

	for (int indexJ = 0; indexJ < 4; indexJ++) {
		for (int indexI = 0; indexI < 4; indexI++) {
			matrixPrint << matrix[indexI + indexJ * 4] << " ";
		}
		if (indexJ < 3) {
			matrixPrint << "\n";
		}
	}

	return matrixPrint.str();
}

Matrix4 Matrix4::operator+(const Matrix4& otherMatrix) const {
	std::vector<float> result(16);
	const std::vector<float> otherMatrixArray = otherMatrix.getArray();

	for (int indexJ = 0; indexJ < 4; indexJ++) {
		for (int indexI = 0; indexI < 4; indexI++) {
			result[indexI + indexJ * 4] = matrix[indexI + indexJ * 4]
					+ otherMatrixArray[indexI + indexJ * 4];
		}
	}

	return Matrix4(result);
}

Matrix4 Matrix4::operator-(const Matrix4& otherMatrix) const {
	std::vector<float> result(16);
	const std::vector<float> otherMatrixArray = otherMatrix.getArray();

	for (int indexJ = 0; indexJ < 4; indexJ++) {
		for (int indexI = 0; indexI < 4; indexI++) {
			result[indexI + indexJ * 4] = matrix[indexI + indexJ * 4]
					- otherMatrixArray[indexI + indexJ * 4];
		}
	}

	return Matrix4(result);
}

Matrix4 Matrix4::operator*(const Matrix4& otherMatrix) const {
	std::vector<float> result(16);
	const std::vector<float> otherMatrixArray = otherMatrix.getArray();

	int indexI, indexJ;
	for (indexJ = 0; indexJ < 4; indexJ++) {
		for (indexI = 0; indexI < 4; indexI++) {
			result[indexI + indexJ * 4] = matrix[0 + indexJ * 4]
					* otherMatrixArray[indexI + 0 * 4]
					+ matrix[1 + indexJ * 4] * otherMatrixArray[indexI + 1 * 4]
					+ matrix[2 + indexJ * 4] * otherMatrixArray[indexI + 2 * 4]
					+ matrix[3 + indexJ * 4] * otherMatrixArray[indexI + 3 * 4];
		}
	}

	return Matrix4(result);
}

void Matrix4::operator=(const Matrix4& otherMatrix) {
	matrix = otherMatrix.getArray();
}

void Matrix4::operator+=(const Matrix4& otherMatrix) {
	std::vector<float> otherMatrixArray = otherMatrix.getArray();

	for (int indexJ = 0; indexJ < 4; indexJ++) {
		for (int indexI = 0; indexI < 4; indexI++) {
			matrix[indexI + indexJ * 4] +=
					otherMatrixArray[indexI + indexJ * 4];
		}
	}
}

void Matrix4::operator-=(const Matrix4& otherMatrix) {
	std::vector<float> otherMatrixArray = otherMatrix.getArray();

	for (int indexJ = 0; indexJ < 4; indexJ++) {
		for (int indexI = 0; indexI < 4; indexI++) {
			matrix[indexI + indexJ * 4] -=
					otherMatrixArray[indexI + indexJ * 4];
		}
	}
}

void Matrix4::operator*=(const Matrix4& otherMatrix) {
	std::vector<float> result(16);
	const std::vector<float> otherMatrixArray = otherMatrix.getArray();

	int indexI, indexJ;
	for (indexJ = 0; indexJ < 4; indexJ++) {
		for (indexI = 0; indexI < 4; indexI++) {
			result[indexI + indexJ * 4] = matrix[0 + indexJ * 4]
					* otherMatrixArray[indexI + 0 * 4]
					+ matrix[1 + indexJ * 4] * otherMatrixArray[indexI + 1 * 4]
					+ matrix[2 + indexJ * 4] * otherMatrixArray[indexI + 2 * 4]
					+ matrix[3 + indexJ * 4] * otherMatrixArray[indexI + 3 * 4];
		}
	}

	matrix = result;
}

Matrix4 Matrix4::operator*(int scalar) const {
	std::vector<float> result(16);

	for (int indexJ = 0; indexJ < 4; indexJ++) {
		for (int indexI = 0; indexI < 4; indexI++) {
			result[indexI + indexJ * 4] = matrix[indexI + indexJ * 4] * scalar;
		}
	}

	return Matrix4(result);
}

Matrix4 Matrix4::operator*(float scalar) const {
	std::vector<float> result(16);

	for (int indexJ = 0; indexJ < 4; indexJ++) {
		for (int indexI = 0; indexI < 4; indexI++) {
			result[indexI + indexJ * 4] = matrix[indexI + indexJ * 4] * scalar;
		}
	}

	return Matrix4(result);
}

void Matrix4::operator*=(int scalar) {
	for (int indexJ = 0; indexJ < 4; indexJ++) {
		for (int indexI = 0; indexI < 4; indexI++) {
			matrix[indexI + indexJ * 4] *= scalar;
		}
	}
}

void Matrix4::operator*=(float scalar) {
	for (int indexJ = 0; indexJ < 4; indexJ++) {
		for (int indexI = 0; indexI < 4; indexI++) {
			matrix[indexI + indexJ * 4] *= scalar;
		}
	}
}

Matrix4 Matrix4::operator+(int scalar) const {
	std::vector<float> result(16);

	for (int indexJ = 0; indexJ < 4; indexJ++) {
		for (int indexI = 0; indexI < 4; indexI++) {
			result[indexI + indexJ * 4] = matrix[indexI + indexJ * 4] + scalar;
		}
	}

	return Matrix4(result);
}

Matrix4 Matrix4::operator+(float scalar) const {
	std::vector<float> result(16);

	for (int indexJ = 0; indexJ < 4; indexJ++) {
		for (int indexI = 0; indexI < 4; indexI++) {
			result[indexI + indexJ * 4] = matrix[indexI + indexJ * 4] + scalar;
		}
	}

	return Matrix4(result);
}

void Matrix4::operator+=(int scalar) {
	for (int indexJ = 0; indexJ < 4; indexJ++) {
		for (int indexI = 0; indexI < 4; indexI++) {
			matrix[indexI + indexJ * 4] += scalar;
		}
	}
}

void Matrix4::operator+=(float scalar) {
	for (int indexJ = 0; indexJ < 4; indexJ++) {
		for (int indexI = 0; indexI < 4; indexI++) {
			matrix[indexI + indexJ * 4] += scalar;
		}
	}
}

Matrix4 Matrix4::operator-(int scalar) const {
	std::vector<float> result(16);

	for (int indexJ = 0; indexJ < 4; indexJ++) {
		for (int indexI = 0; indexI < 4; indexI++) {
			result[indexI + indexJ * 4] = matrix[indexI + indexJ * 4] - scalar;
		}
	}

	return Matrix4(result);
}

Matrix4 Matrix4::operator-(float scalar) const {
	std::vector<float> result(16);

	for (int indexJ = 0; indexJ < 4; indexJ++) {
		for (int indexI = 0; indexI < 4; indexI++) {
			result[indexI + indexJ * 4] = matrix[indexI + indexJ * 4] - scalar;
		}
	}

	return Matrix4(result);
}

void Matrix4::operator-=(int scalar) {
	for (int indexJ = 0; indexJ < 4; indexJ++) {
		for (int indexI = 0; indexI < 4; indexI++) {
			matrix[indexI + indexJ * 4] -= scalar;
		}
	}
}

void Matrix4::operator-=(float scalar) {
	for (int indexJ = 0; indexJ < 4; indexJ++) {
		for (int indexI = 0; indexI < 4; indexI++) {
			matrix[indexI + indexJ * 4] -= scalar;
		}
	}
}

bool Matrix4::operator==(const Matrix4& otherMatrix) const {
	bool isEqual = true;
	const std::vector<float> otherMatrixArray = otherMatrix.getArray();

	int indexI, indexJ;
	for (indexJ = 0; indexJ < 4; indexJ++) {
		for (indexI = 0; indexI < 4; indexI++) {
			if (matrix[indexI + indexJ * 4]
					!= otherMatrixArray[indexI + indexJ * 4]) {
				isEqual = false;
			}
		}
	}

	return isEqual;
}

bool Matrix4::operator!=(const Matrix4& otherMatrix) const {
	return !(*this == otherMatrix);
}

}

