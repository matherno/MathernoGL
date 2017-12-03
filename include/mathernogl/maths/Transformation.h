#pragma once

#include <stdexcept>

#include <mathernogl/maths/Matrix4.h>
#include <mathernogl/maths/MatrixOperations.h>
#include <mathernogl/maths/Vector3D.h>

namespace mathernogl {

enum TransformComponent{
	TRANSFORM_TRANSLATE,
	TRANSFORM_SCALE,
	TRANSFORM_ROTATE,
};

//represents a transformation, with translation, scale, and rotation components
//main transformMatrix must be updated with a call to updateTransformMatrix
class Transformation {
private:
	Matrix4 transformMatrix = Matrix4(1);
	TransformComponent transformOrderFirst = TRANSFORM_SCALE;
	TransformComponent transformOrderSecond = TRANSFORM_ROTATE;
	TransformComponent transformOrderThird = TRANSFORM_TRANSLATE;

public:
	Vector3D translation = {0,0,0};
	Vector3D scale = {1,1,1};
	//rotation (work out quaternians!!!)

	const Matrix4* getTransformMatrix() const { return &transformMatrix; }
	void setTransformOrder(const TransformComponent& first, const TransformComponent& second, const TransformComponent& third);
	void updateTransformMatrix();
	void setTransformMatrix(const Matrix4& matrix);

private:
	void updateTransformMatrixComponent(const TransformComponent& component);

};

}
