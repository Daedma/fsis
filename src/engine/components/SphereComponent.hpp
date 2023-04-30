#pragma once

#include "components/CollisionComponent.hpp"

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