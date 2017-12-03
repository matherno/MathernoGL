/*	Author: Matthew Littlemore	*/
/*	Purpose: Used to wrap a float array as a 4x4 matrix	*/

#pragma once

#include <vector>
#include <iostream>
#include <sstream>

namespace mathernogl {

class Matrix4 {
private:
	std::vector<float> matrix;

public:
	Matrix4();
	Matrix4(float diagonal);
	Matrix4(const Matrix4& matrixToCopy);
	Matrix4(const std::vector<float>& matrixToCopy);

	void setAt(float value, int row, int col);
	float getAt(int row, int col) const;
	void setArray(const std::vector<float>& matrixToCopy);
	const std::vector<float>& getArray() const;
	const float* getCArray() const;
	std::string toString() const;

	Matrix4 operator+(const Matrix4& otherMatrix) const;
	Matrix4 operator-(const Matrix4& otherMatrix) const;
	Matrix4 operator*(const Matrix4& otherMatrix) const;

	void operator=(const Matrix4& otherMatrix);
	void operator+=(const Matrix4& otherMatrix);
	void operator*=(const Matrix4& otherMatrix);
	void operator-=(const Matrix4& otherMatrix);

	Matrix4 operator*(int scalar) const;
	Matrix4 operator*(float scalar) const;

	void operator*=(int scalar);
	void operator*=(float scalar);;

	Matrix4 operator+(int scalar) const;
	Matrix4 operator+(float scalar) const;

	void operator+=(int scalar);
	void operator+=(float scalar);

	Matrix4 operator-(int scalar) const;
	Matrix4 operator-(float scalar) const;

	void operator-=(int scalar);
	void operator-=(float scalar);

	bool operator==(const Matrix4& otherMatrix) const;
	bool operator!=(const Matrix4& otherMatrix) const;
};

}
