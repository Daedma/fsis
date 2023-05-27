#pragma once

#include "actors/Actor.hpp"
#include "Entity.hpp"

class Projectile : public Actor
{
public:
	using OnHitSignature = eastl::function<void(class FSISCharacter*, Entity)>;

	Projectile(World* world);

	void setRadius(float radius);

	Entity getType() const
	{
		return m_type;
	}

	void setType(Entity type);

	void setLifetime(float lifetime)
	{
		m_maxLifetime = lifetime;
	}

	void launch(const Vector3f& speed);

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
};