/**
 * @file TransformTypes.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains declarations of transform types, constants and TSR namespace
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include "Mathter/Matrix.hpp"
#include "Mathter/Vector.hpp"
#include "Mathter/Quaternion.hpp"
#include <EASTL/functional.h>

using Transform = mathter::Matrix<float, 4, 4>;

using Vector4f = mathter::Vector<float, 4>;

using Vector3f = mathter::Vector<float, 3>;

using Vector2f = mathter::Vector<float, 2>;

using Vector3u32 = mathter::Vector<uint32_t, 3>;

using Vector3i32 = mathter::Vector<int32_t, 3>;

using Vector3u64 = mathter::Vector<uint64_t, 3>;

using Vector3i64 = mathter::Vector<int64_t, 3>;

using Vector2u32 = mathter::Vector<uint32_t, 2>;

using Vector2i32 = mathter::Vector<int32_t, 2>;

using Vector2u64 = mathter::Vector<uint64_t, 2>;

using Vector2i64 = mathter::Vector<int64_t, 2>;

using Vector3u = Vector3u32;

using Vector3i = Vector3i32;

using Vector2u = Vector2u32;

using Vector2i = Vector2i32;

namespace eastl
{

	template<typename T>
	struct hash<mathter::Vector<T, 3>>
	{
		size_t operator()(const mathter::Vector<T, 3>& val) const
		{
			return static_cast<size_t>(val.x) ^ static_cast<size_t>(val.y) ^ static_cast<size_t>(val.z);
		}
	};

	template<typename T>
	struct hash<mathter::Vector<T, 2>>
	{
		size_t operator()(const mathter::Vector<T, 2>& val) const
		{
			return static_cast<size_t>(val.x) ^ static_cast<size_t>(val.y);
		}
	};
}

static const Vector3f X_AXIS = Vector3f{ 1.f, 0.f, 0.f };

static const Vector3f Y_AXIS = Vector3f{ 0.f, 1.f, 0.f };

static const Vector3f Z_AXIS = Vector3f{ 0.f, 0.f, 1.f };

static const Vector3f ZERO_VECTOR = Vector3f{ 0.f, 0.f, 0.f };

static const Vector3f TO_PLAYER = Vector3f{ 0.70710678f, 0.70710678f, 0.f };

static const Vector3f TO_RIGHTSIDE = Vector3f{ 0.70710678f, -0.70710678f, 0.f };

static const double SQRT_3 = 1.7320508075688772935274463415059;

static const double INVSQRT_3 = 0.57735026918962576450914878050196;

static const double SQRT_2 = 1.4142135623730950488016887242097;

static const double INVSQRT_2 = 0.70710678118654752440084436210485;

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