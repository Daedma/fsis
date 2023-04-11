#pragma once

#include "core/TransformTypes.hpp"

class Camera
{
	Transform m_transform = mathter::Identity();

	Vector2f m_position = { 0.f, 0.f };

	float m_distance = 0.f;

	float m_scale = 480.f;

public:
	static const Transform ISOMETRIC;

	static const Transform TOPDOWN;

	static const Transform SIDE;

	virtual ~Camera();

	void setScale(float scale) { m_scale = scale; }

	float getScale() const { return m_scale; }

	void setPosition(const Vector2f& position) { m_position = position; }

	const Vector2f& getPosition() const { return m_position; }

	void setDistance(float distance) { m_distance = distance; }

	float getDistance() const { return m_distance; }

	/**
	 * @brief Set the world orientation relative camera (e.g. isometric)
	 *
	 * @param projectionType
	 */
	void setView(const Transform& view) { m_transform = view; }

	const Transform& getView() const { return m_transform; }

	Transform getProjectionMatrix()
	{
		updatePosition();
		Vector3f position{ m_position, 0.f };
		Transform projection = mathter::Orthographic(position - m_scale / 2, position + m_scale / 2, 1.f, -1.f);
		return m_transform * projection;
	}

protected:
	virtual void updatePosition();
};