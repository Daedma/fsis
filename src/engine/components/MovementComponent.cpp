#include "components/MovementComponent.hpp"
#include "core/World.hpp"
#include "actors/Actor.hpp"
#include <cmath>

MovementComponent::MovementComponent(Actor* owner) : ActorComponent(owner)
{
	m_acceleration = { 0.f, 0.f, getWorld()->getGravity() };
	m_lastPosition = owner->getPosition();
	setTickGroup(TickGroups::PREPHYSIC);
}


void MovementComponent::enableGravity()
{
	if (!b_gravity)
	{
		m_acceleration += {0.f, 0.f, getWorld()->getGravity()};
	}
	b_gravity = true;
}

void MovementComponent::disableGravity()
{
	if (b_gravity)
	{
		m_acceleration -= {0.f, 0.f, getWorld()->getGravity()};
	}
	b_gravity = false;
}

void MovementComponent::move(const Vector3f& direction)
{
	m_accumulatedMovement += direction;
}

void MovementComponent::tick(float deltaSeconds)
{
	if (float distance = mathter::Length(m_accumulatedMovement);distance)
	{
		Vector3f movement = m_accumulatedMovement * getSpeed() / distance;
		getOwner()->move(movement * deltaSeconds); // Apply input
	}
	m_speed = (getOwner()->getPosition() - m_lastPosition) / deltaSeconds; // Correct speed
	// m_speed += m_acceleration; // Apply acceleration
	// FIXME Acceleration doesn't work
	// getOwner()->move((m_speed - m_accumulatedMovement) * deltaSeconds); // Apply acceleration difference
	if (b_orientRotationToMovement && !mathter::IsNullvector(m_speed))
	{
		getOwner()->orientByDirection(mathter::Normalize(m_speed));
	}
	m_lastPosition = getOwner()->getPosition();
	m_accumulatedMovement = { 0.f, 0.f, 0.f };
}