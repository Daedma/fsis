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
	m_acceleration = { 0.f, 0.f, getWorld()->getGravity() };
	b_gravity = true;
}

void MovementComponent::disableGravity()
{
	m_acceleration = { 0.f, 0.f, 0.f };
	b_gravity = false;
}

void MovementComponent::move(const Vector3f& direction)
{
	m_accumulatedMovement += direction;
}

void MovementComponent::tick(float deltaSeconds)
{
	m_speed = (getOwner()->getPosition() - m_lastPosition) / deltaSeconds; // Correct speed
	m_lastPosition = getOwner()->getPosition();

	Vector3f movement(0, 0, 0);
	if (float distance = mathter::Length(m_accumulatedMovement);distance)
	{
		movement = m_accumulatedMovement * getSpeed() / distance;
		getOwner()->move(movement * deltaSeconds); // Apply input
	}
	getOwner()->move(m_launchSpeed * deltaSeconds);
	m_speedFromAcceleration *= (m_speed.z < 0.f);
	m_speedFromAcceleration += m_acceleration * deltaSeconds; // Apply acceleration
	getOwner()->move(m_speedFromAcceleration * deltaSeconds);

	Vector3f orientation = { m_speed.x, m_speed.y, 0 };
	if (b_orientRotationToMovement && !mathter::IsNullvector(orientation))
	{
		getOwner()->orientByDirection(mathter::Normalize(orientation));
	}

	m_accumulatedMovement.Set(0.f, 0.f, 0.f);
}