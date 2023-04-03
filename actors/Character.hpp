#pragma once

#include "actors/Actor.hpp"
#include "core/TransformTypes.hpp"

class MovementComponent;

class Character: public Actor
{
	MovementComponent* m_movement = nullptr;

	Character* m_target = nullptr;

public:
	Character(World* world);

	MovementComponent* getMovementComponent() const { return m_movement; }

	virtual void attack();

	Character* setTarget();

	Character* setTarget(Character* target);

	void resetTarget();

protected:
	virtual void onTargeted(Character* instigator);

	virtual void onUntargeted(Character* instigator);

	virtual void onSetTarget(Character* target);
};