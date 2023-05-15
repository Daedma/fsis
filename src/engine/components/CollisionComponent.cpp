#include "components/CollisionComponent.hpp"
#include "core/World.hpp"
#include "core/CollisionResolver.hpp"

CollisionComponent::CollisionComponent()
{
	setTickGroup(TickGroups::PHYSIC);
}

CollisionComponent::CollisionComponent(Actor* owner) : SceneComponent(owner)
{
	setTickGroup(TickGroups::PHYSIC);
	getWorld()->getCollisionResolver()->registry(this);
}

CollisionComponent::CollisionComponent(SceneComponent* parent) : SceneComponent(parent)
{
	setTickGroup(TickGroups::PHYSIC);
	getWorld()->getCollisionResolver()->registry(this);
}

CollisionComponent::~CollisionComponent()
{
	if (getWorld())
	{
		getWorld()->getCollisionResolver()->unregistry(this);
	}
}

void CollisionComponent::tick(float deltaSeconds)
{
	SceneComponent::tick(deltaSeconds);
	getWorld()->getCollisionResolver()->resolve(this);
}

void CollisionComponent::onAttached()
{
	SceneComponent::onAttached();
	getWorld()->getCollisionResolver()->registry(this);
}

Vector3f CollisionComponent::getSupportPoint(const Vector3f& direction) const
{
	Transform t = getWorldTransform();
	mathter::Matrix<float, 3, 3> transposeB(
		t(0, 0), t(1, 0), t(2, 0),
		t(0, 1), t(1, 1), t(2, 1),
		t(0, 2), t(1, 2), t(2, 2)
	);
	return supportMapping(direction * transposeB) * t;
}