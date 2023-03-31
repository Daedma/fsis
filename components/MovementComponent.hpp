#pragma once

#include "components/ActorComponent.hpp"
#include "core/TransformTypes.hpp"

class MovementComponent: public ActorComponent
{
	float m_walkSpeed = 0.f;

	float m_runSpeed = 0.f;

	Vector3f m_speed = { 0.f, 0.f, 0.f };

	Vector3f m_acceleration = { 0.f, 0.f, 0.f };

	Vector3f m_accumulatedMovement = { 0.f, 0.f, 0.f };

	Vector3f m_lastPosition;

	bool m_gravity = true;

	bool m_run = false;

public:
	MovementComponent(Actor* owner);

	void setWalkSpeed(float speed) { m_walkSpeed = speed; }

	void setRunSpeed(float speed) { m_runSpeed = speed; }

	void setAcceleration(const Vector3f& acceleration) { m_acceleration = acceleration; }

	float getWalkSpeed() const { return m_walkSpeed; }

	float getRunSpeed() const { return m_runSpeed; }

	const Vector3f& getCurrentSpeed() const { return m_speed; }

	float getSpeed() const { return m_run ? m_runSpeed : m_walkSpeed; }

	const Vector3f& getAcceleration() const { return m_acceleration; }

	void enableGravity();

	void disableGravity();

	void toggleRun() { m_run = !m_run; }

	bool isRun() const { return m_run; }

	void setRun(bool run) { m_run = run; }

	void launch(const Vector3f& direction) { m_speed = direction * m_walkSpeed; }

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

	virtual void tick(float deltaSeconds) override;

};
