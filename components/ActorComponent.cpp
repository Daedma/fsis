#include "components/ActorComponent.hpp"
#include "actors/Actor.hpp"

void ActorComponent::destroy()
{
	GameObject::destroy();
	if (m_owner)
	{
		m_owner->destroyComponent(this);
	}
}

void ActorComponent::forceDestroy()
{
	m_owner->destroyComponent(this);
}

size_t ActorComponent::getDepth() const
{
	return 0;
}

void ActorComponent::attachToActor(Actor* newOwner)
{
	assert(newOwner != nullptr);
	m_owner = newOwner;
	m_owner->attachComponent(this);
	initWorld(m_owner->getWorld());
	onAttached();
}

void ActorComponent::tick(float deltaSeconds)
{

}

void ActorComponent::onAttached() {}