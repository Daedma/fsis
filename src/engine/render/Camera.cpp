#include "render/Camera.hpp"
#include <cmath>

Camera::~Camera() {}

static Transform getIsometricTransform()
{
	// True isometric
	// https://gamedev.stackexchange.com/questions/181651/why-when-i-do-an-isometric-projection-do-i-get-a-dimmetric-view?rq=1
	Transform rotz = mathter::RotationAxis(mathter::Deg2Rad(45), 2);
	Transform rotx = mathter::RotationAxis(acos(1 / sqrt(3)), 0);
	return rotz * rotx;

	// https://gamedev.stackexchange.com/questions/168899/3d-isometric-projection-matrix
	// Transform rot45y = mathter::RotationAxis<1>(mathter::Deg2Rad(-45));
	// Transform rot30x = mathter::RotationAxis<0>(mathter::Deg2Rad(-30));
	// return rot45y * rot30x;

	// 2:1
	// Transform rotz = mathter::RotationAxis(mathter::Deg2Rad(45), 2);
	// Transform rotx = mathter::RotationAxis(mathter::Deg2Rad(-30), 0);
	// return rotz * rotx;
}

const Transform Camera::ISOMETRIC = getIsometricTransform();

void Camera::updatePosition()
{}