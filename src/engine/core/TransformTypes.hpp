#pragma once
#include "Mathter/Matrix.hpp"
#include "Mathter/Vector.hpp"
#include "Mathter/Quaternion.hpp"

using Transform = mathter::Matrix<float, 4, 4>;

using Vector4f = mathter::Vector<float, 4>;

using Vector3f = mathter::Vector<float, 3>;

using Vector2f = mathter::Vector<float, 2>;

static const Vector3f X_AXIS = Vector3f{ 1.f, 0.f, 0.f };

static const Vector3f Y_AXIS = Vector3f{ 0.f, 1.f, 0.f };

static const Vector3f Z_AXIS = Vector3f{ 0.f, 0.f, 1.f };

static const Vector3f ZERO_VECTOR = Vector3f{ 0.f, 0.f, 0.f };

static const Vector3f TO_PLAYER = Vector3f{ 0.70710678f, 0.70710678f, 0.f };

static const Vector3f TO_RIGHTSIDE = Vector3f{ -0.70710678f, 0.70710678f, 0.f };

using Rotator = mathter::Quaternion<float>;

namespace TSR
{
	inline Vector3f getTranslation(const Transform& transform)
	{
		return { transform(3, 0), transform(3, 1), transform(3, 2) };
	}

	// DONT WORK
	inline Vector3f getScale(const  Transform& transform)
	{
		return {
			mathter::Length(Vector3f{ transform(0, 0), transform(1, 0), transform(2, 0) }),
			mathter::Length(Vector3f{ transform(0, 1), transform(1, 1), transform(2, 1) }),
			mathter::Length(Vector3f{ transform(0, 2), transform(1, 2), transform(2, 2) }) };
		// mathter::Matrix<float, 3, 3> m(transform(0, 0), transform(1, 0), transform(2, 0),
		// 	transform(0, 1), transform(1, 1), transform(2, 1),
		// 	transform(0, 2), transform(1, 2), transform(2, 2));
		// auto [S, V, D] = mathter::DecomposeSVD(m);
		// return { V(0, 0), V(1, 1), V(2, 2) };
		// auto [Q, R] = mathter::DecomposeQR(transform);
		// return { R(0, 0), R(1, 1), R(2, 2) };
	}

	// DONT WORK
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