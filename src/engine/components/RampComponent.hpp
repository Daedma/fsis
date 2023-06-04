/**
 * @file RampComponent.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of RampComponent class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "components/CollisionComponent.hpp"
#include <EASTL/array.h>

/**
 * @brief Provides ramp (inclined surface) collision.
 * Origin (center of inclined edge) located in (0, 0, 0).
 * The slope is directed towards the positive direction of the Ox axis.
 */
class RampComponent : public CollisionComponent
{
	eastl::array<Vector3f, 6> m_vertices;
	/*
	0----1
	|\   |\
	| \  | \
	2--\-3_ \
	   4`````5
	   z
		|
	   /\
	  /  \
	  y   x
	*/
public:
	RampComponent() = default;

	RampComponent(Actor* owner) : CollisionComponent(owner) {}

	RampComponent(SceneComponent* parent) : CollisionComponent(parent) {}

	void setSize(const Vector3f& size)
	{
		EASTL_ASSERT(size.x > 0 && size.y > 0 && size.z > 0);
		m_vertices[0] = Vector3f{ -size.x * 0.5, size.y * 0.5, size.z * 0.5 };
		m_vertices[1] = Vector3f{ -size.x * 0.5, -size.y * 0.5, size.z * 0.5 };
		m_vertices[2] = Vector3f{ -size.x * 0.5, size.y * 0.5, -size.z * 0.5 };
		m_vertices[3] = Vector3f{ -size.x * 0.5, -size.y * 0.5, -size.z * 0.5 };
		m_vertices[4] = Vector3f{ size.x * 0.5, size.y * 0.5, -size.z * 0.5 };
		m_vertices[5] = Vector3f{ size.x * 0.5, -size.y * 0.5, -size.z * 0.5 };
	}

	Vector3f getSize() const
	{
		return { m_vertices[4].x * 2, m_vertices[0].y * 2, m_vertices[0].z * 2 };
	}

protected:
	virtual Vector3f supportMapping(const Vector3f& direction) const override;
};