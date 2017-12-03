#include <mathernogl/maths/RaycastOperations.h>

namespace mathernogl {

Vector3D getRaycastIntersectionOnXZPlane(const Raycast& raycast, const float planeOffset){
	Vector3D intersectPoint;

	if(raycast.direction.y == 0){
		if(raycast.position.y == planeOffset){
			throw mgl_no_intersection("Raycast does not intersect at a point on the XZ plane with offset " + std::to_string(planeOffset) + ". It is parallel and on the plane. ");
		}
		throw mgl_no_intersection("Raycast does not intersect the XZ plane with offset " + std::to_string(planeOffset) + ". It is parallel the plane. ");
	}


	const float tValue = (planeOffset - raycast.position.y) / raycast.direction.y;
	if(tValue < raycast.minTValue || tValue > raycast.maxTValue){
		throw mgl_no_intersection("Raycast does not intersect the XZ plane with offset " + std::to_string(planeOffset) + ". The raycasts bounds prevent it from intersecting ");
	}
	intersectPoint.x = raycast.position.x + raycast.direction.x * tValue;
	intersectPoint.y = planeOffset;
	intersectPoint.z = raycast.position.z + raycast.direction.z * tValue;


	return intersectPoint;
}


}
