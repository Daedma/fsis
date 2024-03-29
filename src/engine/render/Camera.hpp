/**
 * @file Camera.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of Camera class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "core/TransformTypes.hpp"

/**
 * @brief Base class for all isometric camera.
 */
class Camera
{
	Vector2f m_position = { 0.f, 0.f };

	/**
	 * @brief How much units placed in window
	 *
	 */
	float m_scale = 480.f;

public:
	static const Transform ISOMETRIC;

	Camera() = default;

	virtual ~Camera();

	void setScale(float scale) { m_scale = scale; }

	float getScale() const { return m_scale; }

	void setPosition(const Vector2f& position) { m_position = position; }

	const Vector2f& getPosition() const { return m_position; }

	Transform getProjectionMatrix()
	{
		updatePosition();
		Vector3f position{ m_position.x, m_position.y, 0 };
		Transform projection = mathter::Orthographic(position - m_scale * INVSQRT_3, position + m_scale * INVSQRT_3, 1.f, -1.f);
		return ISOMETRIC * projection;
	}

protected:
	virtual void updatePosition();
};