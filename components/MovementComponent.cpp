#include "components/MovementComponent.hpp"
#include "core/World.hpp"
#include "actors/Actor.hpp"

MovementComponent::MovementComponent(Actor* owner): ActorComponent(owner)
{
	m_acceleration = { 0.f, 0.f, getWorld()->getGravity() };
	m_lastPosition = owner->getPosition();
}


void MovementComponent::enableGravity()
{
	if (!m_gravity)
	{
		m_acceleration += {0.f, 0.f, getWorld()->getGravity()};
	}
	m_gravity = true;
}

void MovementComponent::disableGravity()
{
	if (m_gravity)
	{
		m_acceleration -= {0.f, 0.f, getWorld()->getGravity()};
	}
	m_gravity = false;
}

void MovementComponent::move(const Vector3f& direction)
{
	m_accumulatedMovement += direction * getSpeed();
}

void MovementComponent::tick(float deltaSeconds)
{
	m_speed += m_acceleration * deltaSeconds;
	getOwner()->move((m_accumulatedMovement + m_speed) * deltaSeconds);
	//TODO сбрасывать скорость по осям, если не было перемещенийё
}