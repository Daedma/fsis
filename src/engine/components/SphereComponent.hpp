/**
 * @file SphereComponent.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of SphereComponent class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "components/CollisionComponent.hpp"

/**
 * @brief Provide sphere collision with center in (0, 0, 0)
 *
 */
class SphereComponent : public CollisionComponent
{
public:
	SphereComponent() = default;

	SphereComponent(Actor* owner);

	SphereComponent(SceneComponent* parent);

	void setRadius(float radius)
	{
		EASTL_ASSERT(radius >= 0.f);
		m_radius = radius;
	}

	float getRadius() const { return m_radius; }

private:
	float m_radius = 10.f;

protected:

	virtual Vector3f supportMapping(const Vector3f& direction) const override;
};