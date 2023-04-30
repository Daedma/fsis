#include "components/BoxComponent.hpp"
#include <cmath>

BoxComponent::BoxComponent(Actor* owner) : CollisionComponent(owner) {}

BoxComponent::BoxComponent(SceneComponent* parent) : CollisionComponent(parent) {}

static float signum(float arg)
{
	return (0.f < arg) - (0.f > arg);
}

Vector3f BoxComponent::supportMapping(const Vector3f& direction) const
{
	return { signum(direction.x) * m_size.x * 0.5f, signum(direction.y) * m_size.y * 0.5f, signum(direction.z) * m_size.z * 0.5 };
}