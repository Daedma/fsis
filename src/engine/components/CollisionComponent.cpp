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

void CollisionComponent::tick(float deltaSeconds)
{
	getWorld()->getCollisionResolver()->resolve(this);
}

void CollisionComponent::onAttached()
{
	getWorld()->getCollisionResolver()->registry(this);
}