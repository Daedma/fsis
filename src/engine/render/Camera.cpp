#include "render/Camera.hpp"

static Transform getIsometricTransform()
{
	Transform rotm45y = mathter::RotationAxis(mathter::Deg2Rad(-45), 1);
	Transform rotm30x = mathter::RotationAxis(mathter::Deg2Rad(-30), 0);
	return rotm30x * rotm45y;
}

const Transform Camera::ISOMETRIC = getIsometricTransform();