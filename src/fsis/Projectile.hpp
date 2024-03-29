/**
 * @file Projectile.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of Projectile class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "actors/Actor.hpp"
#include "Entity.hpp"

/**
 * @brief A class represents projectile (balls)
 */
class Projectile : public Actor
{
public:
	using OnHitSignature = eastl::function<void(class FSISCharacter*, Entity)>;

	Projectile(World* world);

	void setRadius(float radius);

	float getRadius() const
	{
		return m_radius;
	}

	void setSpeed(float speed);

	float getSpeed() const;

	Entity getType() const
	{
		return m_type;
	}

	void setType(Entity type);

	void setLifetime(float lifetime)
	{
		m_maxLifetime = lifetime;
	}

	void launch(const Vector3f& direction);

	void setOnHit(OnHitSignature handler)
	{
		m_onHit = handler;
	}

	virtual void tick(float deltaSeconds) override;
private:
	float m_radius = 40.f;

	float m_maxLifetime = 10.f;

	float m_curLifetime = 0.f;

	class SphereComponent* m_proxSphere = nullptr;

	class SpriteComponent* m_sprite = nullptr;

	class MovementComponent* m_movement = nullptr;

	class AudioComponent* m_sound = nullptr;

	Entity  m_type = Entity::INFERNAL;

	OnHitSignature m_onHit = nullptr;

	static constexpr int TAG_BALL = 55;
};