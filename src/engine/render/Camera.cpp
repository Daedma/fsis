#include "render/Camera.hpp"
#include <cmath>

Camera::~Camera() {}

static Transform getIsometricTransform()
{
	Transform rotm45z = mathter::RotationAxis(mathter::Deg2Rad(45), 2);
	Transform rotm35x = mathter::RotationAxis(std::asin(1.f / std::sqrt(3.f)), 0);
	return rotm45z * rotm35x;
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