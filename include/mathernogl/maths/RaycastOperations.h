#pragma once

#include <limits>
#include <stdexcept>

#include <mathernogl/maths/Vector3D.h>

namespace mathernogl {

//raycast = position + (direction * t)
struct Raycast{
	Vector3D position;
	Vector3D direction;
	float minTValue = -std::numeric_limits<float>::infinity();
	float maxTValue = std::numeric_limits<float>::infinity();
};

class mgl_no_intersection : public std::runtime_error {
public:
	mgl_no_intersection(std::string message) : std::runtime_error(message) {}
};

Vector3D getRaycastIntersectionOnXZPlane(const Raycast& raycast, const float planeOffset);

}
