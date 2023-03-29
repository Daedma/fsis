#include "actors/Actor.hpp"
#include "core/World.hpp"
#include "EASTL/algorithm.h"

void Actor::tick(float deltaSeconds)
{
	m_lastMovement.Set(0.f, 0.f, 0.f);
}

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

void Actor::move(const Vector3f& direction)
{
	setPosition(direction + getPosition());
	m_lastMovement += direction;
}

void Actor::attachComponent(ActorComponent* component)
{
	if (component->getOwner())
	{
		m_components.emplace_back(component);
	}
	else
	{
		component->attachToActor(this);
	}
}