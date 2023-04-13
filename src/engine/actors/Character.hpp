#pragma once

#include "actors/Actor.hpp"
#include "core/TransformTypes.hpp"

class MovementComponent;
class Controller;

class Character : public Actor
{
	MovementComponent* m_movement = nullptr;

	bool b_possessed = false;

public:
	Character(World* world);

	MovementComponent* getMovementComponent() const { return m_movement; }

	virtual void setupController(Controller* controller);

	bool isPossessed() const { return b_possessed; }

	void setPossessed(bool possessed) { b_possessed = possessed; }
};