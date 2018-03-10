/*	Author: Matthew Littlemore	*/
/*	Purpose: Used to wrap a T array as a 4x4 matrix	*/

#pragma once

#include <vector>
#include <iostream>
#include <sstream>

namespace mathernogl {

template <class T> class TMatrix4 {
private:
	std::vector<T> matrix;

public:
	TMatrix4<T>();
	TMatrix4<T>(T diagonal);
	TMatrix4<T>(const TMatrix4<T>& matrixToCopy);
	TMatrix4<T>(const std::vector<T>& matrixToCopy);

	void setAt(T value, int row, int col);
	T getAt(int row, int col) const;
	void setArray(const std::vector<T>& matrixToCopy);
	const std::vector<T>& getArray() const;
	const T* getCArray() const;
	std::string toString() const;

	TMatrix4<T> operator+(const TMatrix4<T>& otherMatrix) const;
	TMatrix4<T> operator-(const TMatrix4<T>& otherMatrix) const;
	TMatrix4<T> operator*(const TMatrix4<T>& otherMatrix) const;

	void operator=(const TMatrix4<T>& otherMatrix);
	void operator+=(const TMatrix4<T>& otherMatrix);
	void operator*=(const TMatrix4<T>& otherMatrix);
	void operator-=(const TMatrix4<T>& otherMatrix);

	TMatrix4<T> operator*(int scalar) const;
	TMatrix4<T> operator*(T scalar) const;

	void operator*=(int scalar);
	void operator*=(T scalar);;

	TMatrix4<T> operator+(int scalar) const;
	TMatrix4<T> operator+(T scalar) const;

	void operator+=(int scalar);
	void operator+=(T scalar);

	TMatrix4<T> operator-(int scalar) const;
	TMatrix4<T> operator-(T scalar) const;

	void operator-=(int scalar);
	void operator-=(T scalar);

	bool operator==(const TMatrix4<T>& otherMatrix) const;
	bool operator!=(const TMatrix4<T>& otherMatrix) const;
};


template <class T> TMatrix4<T>::TMatrix4() {
  matrix = {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0
  };
  }

template <class T> TMatrix4<T>::TMatrix4(T diagonal) {
  matrix = {
    diagonal, 0, 0, 0,
    0, diagonal, 0, 0,
    0, 0, diagonal, 0,
    0, 0, 0, diagonal
  };
  }

template <class T> TMatrix4<T>::TMatrix4(const std::vector<T>& matrixToCopy) {
  matrix = matrixToCopy;
  }

template <class T> TMatrix4<T>::TMatrix4(const TMatrix4<T>& matrixToCopy) {
  matrix = matrixToCopy.getArray();
  }

template <class T> void TMatrix4<T>::setAt(T value, int row, int col) {
  matrix[col + row * 4] = value;
  }

template <class T> T TMatrix4<T>::getAt(int row, int col) const {
  return matrix[col + row * 4];
  }

template <class T> void TMatrix4<T>::setArray(const std::vector<T>& matrixToCopy) {
  matrix = matrixToCopy;
  }

template <class T> const std::vector<T>& TMatrix4<T>::getArray() const {
  return matrix;
  }

template <class T> const T* TMatrix4<T>::getCArray() const {
  return &matrix[0];
  }

template <class T> std::string TMatrix4<T>::toString() const {
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

template <class T> TMatrix4<T> TMatrix4<T>::operator+(const TMatrix4<T>& otherMatrix) const {
  std::vector<T> result(16);
  const std::vector<T> otherMatrixArray = otherMatrix.getArray();

  for (int indexJ = 0; indexJ < 4; indexJ++) {
    for (int indexI = 0; indexI < 4; indexI++) {
      result[indexI + indexJ * 4] = matrix[indexI + indexJ * 4]
                                    + otherMatrixArray[indexI + indexJ * 4];
      }
    }

  return TMatrix4<T>(result);
  }

template <class T> TMatrix4<T> TMatrix4<T>::operator-(const TMatrix4<T>& otherMatrix) const {
  std::vector<T> result(16);
  const std::vector<T> otherMatrixArray = otherMatrix.getArray();

  for (int indexJ = 0; indexJ < 4; indexJ++) {
    for (int indexI = 0; indexI < 4; indexI++) {
      result[indexI + indexJ * 4] = matrix[indexI + indexJ * 4]
                                    - otherMatrixArray[indexI + indexJ * 4];
      }
    }

  return TMatrix4<T>(result);
  }

template <class T> TMatrix4<T> TMatrix4<T>::operator*(const TMatrix4<T>& otherMatrix) const {
  std::vector<T> result(16);
  const std::vector<T> otherMatrixArray = otherMatrix.getArray();

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

  return TMatrix4<T>(result);
  }

template <class T> void TMatrix4<T>::operator=(const TMatrix4<T>& otherMatrix) {
  matrix = otherMatrix.getArray();
  }

template <class T> void TMatrix4<T>::operator+=(const TMatrix4<T>& otherMatrix) {
  std::vector<T> otherMatrixArray = otherMatrix.getArray();

  for (int indexJ = 0; indexJ < 4; indexJ++) {
    for (int indexI = 0; indexI < 4; indexI++) {
      matrix[indexI + indexJ * 4] +=
        otherMatrixArray[indexI + indexJ * 4];
      }
    }
  }

template <class T> void TMatrix4<T>::operator-=(const TMatrix4<T>& otherMatrix) {
  std::vector<T> otherMatrixArray = otherMatrix.getArray();

  for (int indexJ = 0; indexJ < 4; indexJ++) {
    for (int indexI = 0; indexI < 4; indexI++) {
      matrix[indexI + indexJ * 4] -=
        otherMatrixArray[indexI + indexJ * 4];
      }
    }
  }

template <class T> void TMatrix4<T>::operator*=(const TMatrix4<T>& otherMatrix) {
  std::vector<T> result(16);
  const std::vector<T> otherMatrixArray = otherMatrix.getArray();

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

template <class T> TMatrix4<T> TMatrix4<T>::operator*(int scalar) const {
  std::vector<T> result(16);

  for (int indexJ = 0; indexJ < 4; indexJ++) {
    for (int indexI = 0; indexI < 4; indexI++) {
      result[indexI + indexJ * 4] = matrix[indexI + indexJ * 4] * scalar;
      }
    }

  return TMatrix4<T>(result);
  }

template <class T> TMatrix4<T> TMatrix4<T>::operator*(T scalar) const {
  std::vector<T> result(16);

  for (int indexJ = 0; indexJ < 4; indexJ++) {
    for (int indexI = 0; indexI < 4; indexI++) {
      result[indexI + indexJ * 4] = matrix[indexI + indexJ * 4] * scalar;
      }
    }

  return TMatrix4<T>(result);
  }

template <class T> void TMatrix4<T>::operator*=(int scalar) {
  for (int indexJ = 0; indexJ < 4; indexJ++) {
    for (int indexI = 0; indexI < 4; indexI++) {
      matrix[indexI + indexJ * 4] *= scalar;
      }
    }
  }

template <class T> void TMatrix4<T>::operator*=(T scalar) {
  for (int indexJ = 0; indexJ < 4; indexJ++) {
    for (int indexI = 0; indexI < 4; indexI++) {
      matrix[indexI + indexJ * 4] *= scalar;
      }
    }
  }

template <class T> TMatrix4<T> TMatrix4<T>::operator+(int scalar) const {
  std::vector<T> result(16);

  for (int indexJ = 0; indexJ < 4; indexJ++) {
    for (int indexI = 0; indexI < 4; indexI++) {
      result[indexI + indexJ * 4] = matrix[indexI + indexJ * 4] + scalar;
      }
    }

  return TMatrix4<T>(result);
  }

template <class T> TMatrix4<T> TMatrix4<T>::operator+(T scalar) const {
  std::vector<T> result(16);

  for (int indexJ = 0; indexJ < 4; indexJ++) {
    for (int indexI = 0; indexI < 4; indexI++) {
      result[indexI + indexJ * 4] = matrix[indexI + indexJ * 4] + scalar;
      }
    }

  return TMatrix4<T>(result);
  }

template <class T> void TMatrix4<T>::operator+=(int scalar) {
  for (int indexJ = 0; indexJ < 4; indexJ++) {
    for (int indexI = 0; indexI < 4; indexI++) {
      matrix[indexI + indexJ * 4] += scalar;
      }
    }
  }

template <class T> void TMatrix4<T>::operator+=(T scalar) {
  for (int indexJ = 0; indexJ < 4; indexJ++) {
    for (int indexI = 0; indexI < 4; indexI++) {
      matrix[indexI + indexJ * 4] += scalar;
      }
    }
  }

template <class T> TMatrix4<T> TMatrix4<T>::operator-(int scalar) const {
  std::vector<T> result(16);

  for (int indexJ = 0; indexJ < 4; indexJ++) {
    for (int indexI = 0; indexI < 4; indexI++) {
      result[indexI + indexJ * 4] = matrix[indexI + indexJ * 4] - scalar;
      }
    }

  return TMatrix4<T>(result);
  }

template <class T> TMatrix4<T> TMatrix4<T>::operator-(T scalar) const {
  std::vector<T> result(16);

  for (int indexJ = 0; indexJ < 4; indexJ++) {
    for (int indexI = 0; indexI < 4; indexI++) {
      result[indexI + indexJ * 4] = matrix[indexI + indexJ * 4] - scalar;
      }
    }

  return TMatrix4<T>(result);
  }

template <class T> void TMatrix4<T>::operator-=(int scalar) {
  for (int indexJ = 0; indexJ < 4; indexJ++) {
    for (int indexI = 0; indexI < 4; indexI++) {
      matrix[indexI + indexJ * 4] -= scalar;
      }
    }
}

template <class T> void TMatrix4<T>::operator-=(T scalar) {
  for (int indexJ = 0; indexJ < 4; indexJ++) {
    for (int indexI = 0; indexI < 4; indexI++) {
      matrix[indexI + indexJ * 4] -= scalar;
      }
    }
}

template <class T> bool TMatrix4<T>::operator==(const TMatrix4<T>& otherMatrix) const {
  bool isEqual = true;
  const std::vector<T> otherMatrixArray = otherMatrix.getArray();

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

template <class T> bool TMatrix4<T>::operator!=(const TMatrix4<T>& otherMatrix) const {
  return !(*this == otherMatrix);
}

typedef TMatrix4<double> Matrix4;

}
