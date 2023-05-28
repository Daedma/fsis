#pragma once

#include "components/ActorComponent.hpp"
#include "core/TransformTypes.hpp"

/**
 * @brief Class control movements of owner actor.
 * Apply all input movements via this component.
 */
class MovementComponent : public ActorComponent
{
	float m_walkSpeed = 0.f;

	float m_runSpeed = 0.f;

	Vector3f m_speed = { 0.f, 0.f, 0.f }; // Speed at this moment

	Vector3f m_acceleration = { 0.f, 0.f, 0.f };

	Vector3f m_speedFromAcceleration = { 0.f, 0.f, 0.f };

	Vector3f m_launchSpeed = { 0.f, 0.f, 0.f };

	Vector3f m_accumulatedMovement = { 0.f, 0.f, 0.f };

	Vector3f m_lastPosition;

	bool b_gravity = true;

	bool b_run = false;

	bool b_orientRotationToMovement = false;

	bool b_launch = false;

public:
	MovementComponent(Actor* owner);

	// NOTE Default constructor doesn't make sense

	/**
	 * @brief Set the Orient Rotation To Movement
	 *
	 * @param orientRotationToMovement
	 */
	void setOrientRotationToMovement(bool orientRotationToMovement) { b_orientRotationToMovement = orientRotationToMovement; }

	/**
	 * @brief Get the Orient Rotation To Movement object
	 *
	 * @return true
	 * @return false
	 */
	bool getOrientRotationToMovement() const { return b_orientRotationToMovement; }

	/**
	 * @brief Set the Walk Speed
	 *
	 * @param speed new speed value
	 */
	void setWalkSpeed(float speed) { m_walkSpeed = speed; }

	/**
	 * @brief Set the Run Speed
	 *
	 * @param speed new speed value
	 */
	void setRunSpeed(float speed) { m_runSpeed = speed; }

	/**
	 * @brief Add acceleration to current
	 *
	 * @param acceleration acceleration value
	 */
	void addAcceleration(const Vector3f& acceleration) { m_acceleration += acceleration; }

	/**
	 * @brief Get the Walk Speed
	 *
	 * @return walk speed
	 */
	float getWalkSpeed() const { return m_walkSpeed; }

	/**
	 * @brief Get the Run Speed
	 *
	 * @return run speed
	 */
	float getRunSpeed() const { return m_runSpeed; }

	/**
	 * @brief Get the actual current speed vector
	 *
	 * @return current speed
	 */
	const Vector3f& getCurrentSpeed() const { return m_speed; }

	/**
	 * @brief Get the speed at which the actor is moving
	 *
	 * @return speed value
	 */
	float getSpeed() const { return b_run ? m_runSpeed : m_walkSpeed; }

	/**
	 * @brief Get the Acceleration
	 *
	 * @return acceleration value
	 */
	const Vector3f& getAcceleration() const { return m_acceleration; }

	/**
	 * @brief Enable gravity for this actor
	 *
	 * @note Gravity is enable by default
	 */
	void enableGravity();

	/**
	 * @brief Disable gravity for this actor
	 *
	 * @note Gravity is enable by default
	 */
	void disableGravity();

	/**
	 * @brief If actor is running at this moment it will walk,
	 * if actor is walking at this moment it will run
	 *
	 */
	void toggleRun() { b_run = !b_run; }

	/**
	 * @brief Actor is running?
	 *
	 * @return true - Actor is running
	 * @return false - Actor is walking
	 */
	bool isRun() const { return b_run; }

	/**
	 * @brief Set the Run
	 *
	 * @param run
	 */
	void setRun(bool run) { b_run = run; }

	/**
	 * @brief Launch actor in given @p direction
	 * with current speed
	 *
	 * @param direction vector in direction
	 * @note If you toggle run after launch - speed will not change
	 */
	void launch(const Vector3f& direction)
	{
		m_launchSpeed = direction * getSpeed();
		b_launch = true;
	}

	void stop()
	{
		m_launchSpeed = { 0, 0, 0 };
		b_launch = false;
	}

	void abortAllMovements()
	{
		m_launchSpeed = { 0, 0, 0 };
		m_accumulatedMovement = { 0, 0, 0 };
		m_speedFromAcceleration = { 0, 0, 0 };
		disableGravity();
	}

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

	/**
	 * @brief Called every frame.
	 * Apply all movements.
	 *
	 * @param deltaSeconds time in seconds since last frame
	 */
	virtual void tick(float deltaSeconds) override;

};
