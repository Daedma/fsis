/**
 * @file ActorComponent.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains declaration of ActorComponent class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once 
#include "core/GameObject.hpp"

class Actor;

/**
 * @brief Base class for all components.
 * Actor is owner of component. Component lifetime is actor depends.
 * Each component may has only one owner.
 */
class ActorComponent : public GameObject
{
protected:
	Actor* m_owner = nullptr;

public:
	/**
	 * @brief Construct a new Actor Component object.
	 * @param owner
	 * @note Doesn't attach component to actor
	 */
	ActorComponent(Actor* owner);

	ActorComponent() = default;

	virtual ~ActorComponent();

	/**
	 * @brief Get the Owner object
	 * @return Actor, which component attached to
	 */
	Actor* getOwner() const { return m_owner; }

	/**
	 * @brief Attach this component to actor
	 * @param newOwner
	 */
	virtual void attachToActor(Actor* newOwner);

	/**
	 * @brief Called every frame
	 * @param deltaSeconds Time in seconds since last frame
	 */
	virtual void tick(float deltaSeconds) override;

	/**
	 * @brief Force to remove this actor from owner world
	 * on the next tick
	 */
	virtual void destroy() override;

	/**
	 * @brief Destroy this component immediately
	 * @note Not recomended to use. Use ActorComponent::destroy instead
	 */
	virtual void forceDestroy();

	/**
	 * @brief Get the Depth of the object
	 * @return depth of this component in hierarchy
	 * @note Root component and actor components always return 0
	 */
	virtual size_t getDepth() const;

protected:
	/**
	 * @brief Called after attach
	 */
	virtual void onAttached();
};