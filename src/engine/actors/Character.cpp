#include "actors/Character.hpp"
#include "components/MovementComponent.hpp"

Character::Character(World* world) : Actor(world), m_movement(new MovementComponent(this))
{
	m_movement->attachToActor(this);
	m_movement->setOrientRotationToMovement(true);
}

void Character::setupController(Controller* controller)
{

}

// Character* Character::setTarget(Character* target)
// {
// 	resetTarget();
// 	if (target != nullptr)
// 	{
// 		m_movement->setOrientRotationToMovement(false);
// 		m_target = target;
// 		Vector3f lookDirection = target->getPosition() - getPosition();
// 		orientByDirection(mathter::Normalize(lookDirection));
// 		onSetTarget(m_target);
// 		m_target->onTargeted(this);
// 	}
// 	return m_target;
// }

// void Character::resetTarget()
// {
// 	if (m_target)
// 	{
// 		m_target->onUntargeted(this);
// 		m_target = nullptr;
// 		m_movement->setOrientRotationToMovement(true);
// 	}
// }

// Character* Character::setTarget()
// {

// }