#pragma once

#include "components/ActorComponent.hpp"
#include "core/TransformTypes.hpp"

class MovementComponent: public ActorComponent
{
	float speed;

public:
	MovementComponent(Actor* owner);

	/**
	 * @brief Get the Last Movement
	 *
	 * @return last movement vector
	 */
	const Vector3f& getLastMovement() const;

	/**
	 * @brief move owner actor
	 *
	 * @param direction normalized vector
	 */
	void move(const Vector3f& direction);


};