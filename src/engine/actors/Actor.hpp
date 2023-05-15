#pragma once
// #include "components/SceneComponent.hpp"
#include "core/GameObject.hpp"
#include "core/TransformTypes.hpp"
#include "actors/ActorGroups.hpp"
#include "EASTL/unique_ptr.h"
#include "EASTL/vector.h"

class World;
class SceneComponent;
class ActorComponent;

/**
 * @brief Base class for all objects placing in the world.
 * All world transormation is defined by root component.
 * Root component has no parent, so root component must have only
 * world (absolute) transform (no relative).
 * World is owner of all actors.
 *
 */
class Actor : public GameObject
{
protected:
	/**
	 * @brief Define transform of this actor (scale, position, rotation)
	 *
	 */
	eastl::unique_ptr<SceneComponent> m_rootComponent;

public:
	/**
	 * @brief Construct a new Actor object
	 *
	 * @param world owner world
	 */
	Actor(World* world);

	virtual ~Actor();

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

	/**
	 * @brief Get the root component
	 *
	 * @return root component
	 */
	SceneComponent* getRootComponent() { return m_rootComponent.get(); }

	/**
	 * @brief Get the root component
	 *
	 * @return root  component
	 */
	const SceneComponent* getRootComponent() const { return m_rootComponent.get(); }

	/**
	 * @brief Attach actor component to this actor
	 *
	 * @param component component to attached
	 */
	void attachComponent(ActorComponent* component);

	/**
	 * @brief Destroy own actor component immediately
	 *
	 * @param comp component to destroy
	 * @note Not recomended to use. Use ActorComponent::destroy instead
	 */
	void destroyComponent(ActorComponent* comp);

	/**
	 * @brief Get the Forward Vector object
	 *
	 * @return const Vector3f&
	 */
	Vector3f getForwardVector() const;

	/**
	 * @brief Set the position
	 *
	 * @param position
	 */
	void setPosition(const Vector3f& position);

	/**
	 * @brief Set the scale
	 *
	 * @param scale
	 */
	void setScale(const Vector3f& scale);

	/**
	 * @brief Set the rotation
	 *
	 * @param rotation
	 */
	void setRotation(const Rotator& rotation);

	/**
	 * @brief Get the position
	 *
	 * @return position vector
	 */
	const Vector3f& getPosition() const;

	/**
	 * @brief Get the scale
	 *
	 * @return scale vector
	 */
	const Vector3f& getScale() const;

	/**
	 * @brief Get the rotation
	 *
	 * @return rotation rotator
	 */
	const Rotator& getRotation() const;

	/**
	 * @brief Move actor to given direction
	 *
	 * @param direction new position relative to current
	 */
	void move(const Vector3f& direction);

	/**
	 * @brief Rotate actor
	 *
	 * @param rotation
	 */
	void rotate(const Rotator& rotation);

	/**
	 * @brief Orient this actor by vector
	 *
	 * @param direction normalized vector of direction
	 */
	void orientByDirection(const Vector3f& direction);

	/**
	 * @brief Get the last movement vector
	 *
	 * @return last movement vector
	 */
	const Vector3f& getLastMovement() const { return m_lastMovement; }

	ActorsGroups getGroup() const { return m_group; }

	void setOnFloor(bool onFloor) { b_onFloor = onFloor; }

	bool isOnFloor() const { return b_onFloor; }

protected:

	void setGroup(ActorsGroups group) { m_group = group; }

private:
	/**
	 * @brief Accumulate all moves at this tick
	 *
	 */
	Vector3f m_lastMovement;

	/**
	 * @brief Components are owning by this actor
	 *
	 */
	eastl::vector<eastl::unique_ptr<ActorComponent>> m_components;

	ActorsGroups m_group = ActorsGroups::ACTORS;

	bool b_onFloor = false;

};