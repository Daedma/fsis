#include "actors/Character.hpp"
#include "components/MovementComponent.hpp"

Character::Character(World* world) : Actor(world), m_movement(new MovementComponent(this))
{
	setGroup(ActorsGroups::CHARACTERS);
	m_movement->attachToActor(this);
	m_movement->setOrientRotationToMovement(true);
}

void Character::setupController(Controller* controller)
{

}
