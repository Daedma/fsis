#pragma once

#include "ActorComponent.hpp"
#include "core/TransformTypes.hpp"
#include "EASTL/vector.h"
#include "EASTL/unique_ptr.h"
#include "Mathter/Matrix.hpp"
#include "Mathter/Vector.hpp"
#include "Mathter/Quaternion.hpp"

/**
 * @brief Component with transform (position, scale, rotation).
 * It can have few childs and only one parent SceneComponent.
 * Owner of SceneComponent - is other SceneComponent.
 * Each actor must have only one SceneCompoenent (rootComponent),
 * that is parent for any other SceneComponent and defines all actor tranforms.
 * Actor must use soft link to access to SceneComponents.
 */
class SceneComponent: public ActorComponent
{
private:
	eastl::vector<eastl::unique_ptr<SceneComponent>> m_childs;

	SceneComponent* m_parent = nullptr;

	// Transforms
	/**
	 * @brief Relative transform matrix
	 *
	 */
	Transform m_transform = mathter::Identity();

	/**
	 * @brief Relative position
	 *
	 */
	Vector3f m_position = { 0.f, 0.f, 0.f };

	/**
	 * @brief Relative scale
	 *
	 */
	Vector3f m_scale = { 1.f, 1.f, 1.f };

	/**
	 * @brief Relative rotation
	 *
	 */
	Rotator m_rotation = mathter::Identity();

public:
	/**
	 * @brief Construct a new Scene Component object
	 *
	 * @param parent
	 */
	SceneComponent(SceneComponent* parent): ActorComponent(parent->getOwner()), m_parent(parent)
	{
		parent->attachComponent(this);
	}

	SceneComponent() = default;

	/**
	 * @brief Construct a new Scene Component object
	 *
	 * @param owner
	 *
	 * @note For rootComponent
	 */
	SceneComponent(Actor* owner): ActorComponent(owner) {}

	virtual void attachToActor(Actor* actor) override;

	/**
	 * @brief Destroy child immediately
	 *
	 * @param child child to destroy
	 * @note Not recommended to use. Use SceneComponent::destroy instead.
	 */
	void destroyChild(SceneComponent* child);

	virtual void forceDestroy() override;

	/**
	 * @brief Get the Depth of the object
	 *
	 * @return depth of this component in hierarchy
	 * @note Root component always return 0
	 */
	virtual size_t getDepth() const override;

	/**
	 * @brief Get the World Transform of the object
	 *
	 * @return World transform matrix
	 */
	Transform getWorldTransform() const
	{
		return m_parent ? m_parent->getWorldTransform() * m_transform : m_transform;
	}

/**
 * @brief Get the relative transform of the object
 *
 * @return Relative transform matrix
 */
	const Transform& getRelativeTransform() const { return m_transform; }

	/**
	 * @brief Set the relative position
	 *
	 * @param position
	 */
	void setPosition(const Vector3f& position);

	/**
	 * @brief Set the relative scale
	 *
	 * @param scale
	 */
	void setScale(const Vector3f& scale);

	/**
	 * @brief Set the relative rotation
	 *
	 * @param rotation
	 */
	void setRotation(const Rotator& rotation);

	/**
	 * @brief Get the relative position
	 *
	 * @return relative position vector
	 */
	const Vector3f& getPosition() const { return m_position; }

	/**
	 * @brief Get the relative scale
	 *
	 * @return relative scale vector
	 */
	const Vector3f& getScale() const { return m_scale; }

	/**
	 * @brief Get the relative rotation
	 *
	 * @return relative rotation rotator
	 */
	const Rotator& getRotation() const { return m_rotation; }

	/**
	 * @brief Attach @p child component to this component
	 *
	 * @param child
	 */
	void attachComponent(SceneComponent* child);

	void attachToComponent(SceneComponent* parent)
	{
		parent->attachComponent(this);
	}
};