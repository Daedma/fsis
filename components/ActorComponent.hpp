#pragma once 
#include "core/GameObject.hpp"

class Actor;

/**
 * @brief Base class for all components.
 * Actor is owner of component. Component lifetime is actor depends.
 * Each component may has only one owner.
 */
class ActorComponent: public GameObject
{
private:
	Actor* m_owner = nullptr;

public:
	ActorComponent(Actor* owner);

	/**
	 * @brief Get the Owner object
	 *
	 * @return Actor, which component attached to
	 */
	Actor* getOwner() const { return m_owner; }

	/**
	 * @brief Attach this component to actor
	 *
	 * @param newOwner
	 */
	void attachToActor(Actor* newOwner);

	/**
	 * @brief Called every frame
	 *
	 * @param deltaSeconds Time in seconds since last frame
	 */
	virtual void tick(float deltaSeconds) override;

	/**
	 * @brief Force to remove this actor from owner world
	 * on the next tick
	 *
	 */
	virtual void destroy() override;

protected:
	/**
	 * @brief Called after attach
	 *
	 */
	virtual void onAttached();
};