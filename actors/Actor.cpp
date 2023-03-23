#include "actors/Actor.hpp"
#include "core/World.hpp"

void Actor::tick(float deltaSeconds)
{
	destroyComponents();
}

void Actor::destroy()
{
	GameObject::destroy();
	getWorld()->destroyActor(this);
}

void Actor::destroyComponent(ActorComponent* component)
{
	m_toDestroyOnNextTick.emplace(component);
}

void Actor::destroyComponents()
{
	while (!m_toDestroyOnThisTick.empty())
	{
		ActorComponent* curComponent = m_toDestroyOnThisTick.back();
		m_components.erase(curComponent);
	}
}