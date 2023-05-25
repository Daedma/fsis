#pragma once

#include "actors/Actor.hpp"
#include "core/TransformTypes.hpp"

class MovementComponent;
class Controller;

class Character : public Actor
{
	MovementComponent* m_movement = nullptr;

	class Controller* m_controller = nullptr;

public:
	Character(World* world);

	MovementComponent* getMovementComponent() const { return m_movement; }

	virtual void setupController(Controller* controller);

	bool isPossessed() const { return m_controller; }

	void setController(class Controller* controller) { m_controller = controller; }

	class Controller* getController() const { return m_controller; }
};