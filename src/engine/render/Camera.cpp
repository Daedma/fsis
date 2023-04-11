#include "render/Camera.hpp"

Camera::~Camera() {}

static Transform getIsometricTransform()
{
	Transform rotm45y = mathter::RotationAxis(mathter::Deg2Rad(-45), 1);
	Transform rotm30x = mathter::RotationAxis(mathter::Deg2Rad(-30), 0);
	return rotm30x * rotm45y;
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