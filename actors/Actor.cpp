#include "actors/Actor.hpp"
#include "core/World.hpp"
#include "EASTL/algorithm.h"

void Actor::tick(float deltaSeconds)
{}

void Actor::destroy()
{
	GameObject::destroy();
	getWorld()->destroyActor(this);
}

void Actor::destroyComponent(ActorComponent* component)
{
	m_components.erase(eastl::find_if(m_components.begin(), m_components.end(),
		[component](const eastl::unique_ptr<ActorComponent>& rhs) {
			return rhs.get() == component;
		}));
}

