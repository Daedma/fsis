#include "render/Camera.hpp"
#include <cmath>

Camera::~Camera() {}

static Transform getIsometricTransform()
{
	// True isometric
	// https://gamedev.stackexchange.com/questions/181651/why-when-i-do-an-isometric-projection-do-i-get-a-dimmetric-view?rq=1
	Transform rotm45z = mathter::RotationAxis(mathter::Deg2Rad(45), 2);
	// Transform rotm35x = mathter::RotationAxis(mathter::Deg2Rad(35.264), 0);
	Transform rotm35x = mathter::RotationAxis(cos(1 / sqrt(3)), 0);
	return rotm45z * rotm35x;

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

const Transform Camera::TOPDOWN = mathter::Identity();

const Transform Camera::SIDE{
	1.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 1.f
};
// Swap y and z axises

void Camera::updatePosition()
{}