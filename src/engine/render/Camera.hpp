#pragma once

#include "core/TransformTypes.hpp"

class Camera
{
	Transform m_transform = mathter::Identity();

	Vector2f m_position = { 0.f, 0.f };

	float m_distance = 0.f;

	float m_scale = 1.f;

public:
	static const Transform ISOMETRIC;

	void setScale(float scale) { m_scale = scale; }

	void setPosition(const Vector3f& position) { m_position = position.xy; }

	/**
	 * @brief Set the pre-projection transform
	 *
	 * @param transform
	 */
	void setTransform(const Transform& transform) { m_transform = transform; }

	const Transform& getTransform() const { return m_transform; }

	Transform getProjectionMatrix()
	{
		Vector3f position{ m_position, 0.f };
		Transform projection = mathter::Orthographic(position - m_scale / 2, position + m_scale / 2, 1.f, -1.f);
		return m_transform * projection;
	}
};