#pragma once
#include "Mathter/Matrix.hpp"
#include "Mathter/Vector.hpp"
#include "Mathter/Quaternion.hpp"

using Transform = mathter::Matrix<float, 4, 4>;

using Vector3f = mathter::Vector<float, 3>;

using Vector2f = mathter::Vector<float, 2>;

using Rotator = mathter::Quaternion<float>;

namespace TSR
{
	inline Vector3f getTranslation(const Transform& transform)
	{
		return { transform(3, 0), transform(3, 1), transform(3, 2) };
	}

	inline Vector3f getScale(const  Transform& transform)
	{
		return {
			mathter::Length(Vector3f{ transform(0, 0), transform(1, 0), transform(2, 0) }),
			mathter::Length(Vector3f{ transform(0, 1), transform(1, 1), transform(2, 1) }),
			mathter::Length(Vector3f{ transform(0, 2), transform(1, 2), transform(2, 2) }) };
	}

	inline Transform getRotation(const Transform& transform, const Vector3f& scale)
	{
		return Transform{
			transform(0, 0) / scale.x, transform(0, 1) / scale.y, transform(0, 2) / scale.z, 0.f,
			transform(1, 0) / scale.x, transform(1, 1) / scale.y, transform(1, 2) / scale.z, 0.f,
			transform(2, 0) / scale.x, transform(2, 1) / scale.y, transform(2, 2) / scale.z, 0.f,
			0.f, 0.f, 0.f, 1.f
		};
	}

	inline Transform getRotation(const Transform& transform)
	{
		return getRotation(transform, getScale(transform));
	}

}