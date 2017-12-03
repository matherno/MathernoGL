#include <mathernogl/maths/Transformation.h>

namespace mathernogl {

//updates the transformMatrix to reflect the current total transformation of the held transform components of order specified by transformOrder* fields
void Transformation::updateTransformMatrix(){
	transformMatrix = Matrix4(1);
	updateTransformMatrixComponent(transformOrderFirst);
	updateTransformMatrixComponent(transformOrderSecond);
	updateTransformMatrixComponent(transformOrderThird);
}

//used by updateTransformMatrix to update the transform matrix with the given component
void Transformation::updateTransformMatrixComponent(const TransformComponent& component){
	switch(component){
	case TRANSFORM_TRANSLATE:
		transformMatrix *= matrixTranslate(translation);
		break;
	case TRANSFORM_SCALE:
		transformMatrix *= matrixScale(scale);
		break;
	case TRANSFORM_ROTATE:
		break;
	}
}

//sets the order of the transform components in the total transform
//throws exception if any of the parameters are equal
void Transformation::setTransformOrder(const TransformComponent& first, const TransformComponent& second, const TransformComponent& third){
	if(first != second && first != third && second != third){
		transformOrderFirst = first;
		transformOrderSecond = second;
		transformOrderThird = third;
	}
	else{
		throw std::runtime_error("Trying to set transform order... parameters must not be equal. ");
	}
}

void Transformation::setTransformMatrix(const Matrix4& matrix){
	transformMatrix = matrix;
}

}
