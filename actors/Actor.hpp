#pragma once
#include "components/SceneComponent.hpp"
#include "core/GameObject.hpp"

class World;

class Actor: public GameObject
{
protected:
	SceneComponent m_rootComponent;

public:
	/**
	 * @brief Construct a new Actor object
	 *
	 * @param world owner world
	 */
	Actor(World* world);

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
	SceneComponent* getRootComponent() { return &m_rootComponent; }

	/**
	 * @brief Get the root component
	 *
	 * @return root  component
	 */
	const SceneComponent* getRootComponent() const { return &m_rootComponent; }

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
	const Vector3f& getPosition() const { return m_rootComponent.getPosition(); }

	/**
	 * @brief Get the scale
	 *
	 * @return scale vector
	 */
	const Vector3f& getScale() const { return m_rootComponent.getScale(); }

	/**
	 * @brief Get the rotation
	 *
	 * @return rotation rotator
	 */
	const Rotator& getRotation() const { return m_rootComponent.getRotation(); }
};