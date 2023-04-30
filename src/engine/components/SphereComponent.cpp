#include "components/SphereComponent.hpp"

SphereComponent::SphereComponent(Actor* owner) :CollisionComponent(owner) {}

SphereComponent::SphereComponent(SceneComponent* parent) :CollisionComponent(parent) {}

Vector3f SphereComponent::supportMapping(const Vector3f& direction) const
{
	float lenght = mathter::Length(direction);
	if (lenght == 0.) return ZERO_VECTOR;
	return m_radius / lenght * direction;
}