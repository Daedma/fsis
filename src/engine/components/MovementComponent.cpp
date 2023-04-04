#include "components/MovementComponent.hpp"
#include "core/World.hpp"
#include "actors/Actor.hpp"
#include <cmath>

MovementComponent::MovementComponent(Actor* owner): ActorComponent(owner)
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
	m_accumulatedMovement += direction * getSpeed();
}

void MovementComponent::tick(float deltaSeconds)
{
	getOwner()->move(m_accumulatedMovement * deltaSeconds); // Apply input
	m_speed = (getOwner()->getPosition() - m_lastPosition) / deltaSeconds; // Correct speed
	m_speed += m_acceleration * deltaSeconds; // Apply acceleration
	getOwner()->move((m_speed - m_accumulatedMovement) * deltaSeconds); // Apply acceleration difference
	if (b_orientRotationToMovement)
	{
		float length = mathter::Length(m_speed);
		float pitch = std::asin(m_speed.z / length);
		float yaw = std::asin(m_speed.x / length);
		getOwner()->setRotation(mathter::RotationRPY(0.f, pitch, yaw));
	}
	m_lastPosition = getOwner()->getPosition();
	m_accumulatedMovement = { 0.f, 0.f, 0.f };
}