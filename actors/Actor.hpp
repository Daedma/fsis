#pragma once
#include "components/SceneComponent.hpp"
#include "core/GameObject.hpp"
#include "EASTL/unique_ptr.h"
#include "EASTL/unordered_set.h"

class World;

class Actor: public GameObject
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
	Actor(World* world): GameObject(world), m_rootComponent(new SceneComponent(this)) {}

	/**
	 * @brief Called every frame
	 *
	 * @param deltaSeconds Time in seconds since last frame
	 */
	virtual void tick(float deltaSeconds) override;

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

	void destroyComponent(ActorComponent* comp);

	/**
	 * @brief Set the position
	 *
	 * @param position
	 */
	void setPosition(const Vector3f& position) { m_rootComponent->setPosition(position); }

	/**
	 * @brief Set the scale
	 *
	 * @param scale
	 */
	void setScale(const Vector3f& scale) { m_rootComponent->setPosition(scale); }

	/**
	 * @brief Set the rotation
	 *
	 * @param rotation
	 */
	void setRotation(const Rotator& rotation) { m_rootComponent->setRotation(rotation); }

	/**
	 * @brief Get the position
	 *
	 * @return position vector
	 */
	const Vector3f& getPosition() const { return m_rootComponent->getPosition(); }

	/**
	 * @brief Get the scale
	 *
	 * @return scale vector
	 */
	const Vector3f& getScale() const { return m_rootComponent->getScale(); }

	/**
	 * @brief Get the rotation
	 *
	 * @return rotation rotator
	 */
	const Rotator& getRotation() const { return m_rootComponent->getRotation(); }

private:
	eastl::unordered_set<eastl::unique_ptr<ActorComponent>> m_components;

};