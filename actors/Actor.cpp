#include "actors/Actor.hpp"
#include "components/SceneComponent.hpp"
#include "core/World.hpp"
#include "EASTL/algorithm.h"

Actor::Actor(World* world): GameObject(world), m_rootComponent(new SceneComponent(this)) {}

Actor::~Actor() {}

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

void Actor::setPosition(const Vector3f& position) { m_rootComponent->setPosition(position); }

void Actor::setScale(const Vector3f& scale) { m_rootComponent->setPosition(scale); }

void Actor::setRotation(const Rotator& rotation) { m_rootComponent->setRotation(rotation); }

const Vector3f& Actor::getPosition() const { return m_rootComponent->getPosition(); }

const Vector3f& Actor::getScale() const { return m_rootComponent->getScale(); }

const Rotator& Actor::getRotation() const { return m_rootComponent->getRotation(); }