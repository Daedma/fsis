#include "Projectile.hpp"
#include "FSISCharacter.hpp"
#include "Components.hpp"
#include "Core.hpp"

Projectile::Projectile(World* world) : Actor(world)
{
	m_movement = new MovementComponent(this);
	m_movement->setRunSpeed(600);
	m_movement->setWalkSpeed(600);
	m_movement->disableGravity();
	m_movement->setOrientRotationToMovement(false);
	m_movement->attachToActor(this);

	m_proxSphere = new SphereComponent();
	m_proxSphere->setRadius(m_radius);
	m_proxSphere->setOverlapRule(ActorsGroups::ALL, CollisionComponent::OverlapRules::OVERLAP);
	m_proxSphere->setOnOverlapEvent([this](CollisionComponent* comp) {
		if (comp->getTag() != 0) return;
		m_sound->stopPlaying();
		m_sound->playSound("Fireball_explosion_3.wav");
		m_sound->setOnPlayingFinished("Fireball_explosion_3.wav", [this](float duration) {
			destroy();
			});
		m_sprite->hide();
		m_movement->deactivate();
		m_proxSphere->setOverlapRule(ActorsGroups::ALL, CollisionComponent::OverlapRules::IGNORE);
		m_proxSphere->deactivate();
		FSISCharacter* hit = dynamic_cast<FSISCharacter*>(comp->getOwner());
		if (hit && m_onHit)
		{
			m_onHit(hit, m_type);
		}
		});
	m_proxSphere->setTag(55);
	m_proxSphere->attachToActor(this);

	m_sprite = new SpriteComponent();
	m_sprite->loadTexture("Ball_Fair.png");
	m_sprite->setHeight(m_radius * INVSQRT_3);
	m_sprite->attachToActor(this);

	m_sound = new AudioComponent();
	m_sound->playSound("Fireball_loop.wav", true);
	m_sound->playSound("Fireball_throw.wav");
	m_sound->attachToActor(this);

}

void Projectile::setSpeed(float speed)
{
	m_movement->setWalkSpeed(speed);
	m_movement->setRunSpeed(speed);
}

float Projectile::getSpeed() const
{
	return m_movement->getRunSpeed();
}

void Projectile::setRadius(float radius)
{
	m_radius = radius;
	m_proxSphere->setRadius(radius);
	m_sprite->setHeight(radius * INVSQRT_3);
}

void Projectile::setType(Entity type)
{
	m_type = type;
	switch (type)
	{
	case Entity::INFERNAL:
		m_sprite->loadTexture("Ball_Fair.png");
		break;
	case Entity::DISEASED:
		m_sprite->loadTexture("Ball_Warped_sterm.png");
		break;
	case Entity::UNHOLY:
		m_sprite->loadTexture("Ball_Crying_obsidian.png");
		break;
	case Entity::PURIFIED:
		m_sprite->loadTexture("Ball_Sculk.png");
		break;
	case Entity::DIVINE:
		m_sprite->loadTexture("Ball_Glowstone.png");
		break;
	case Entity::QUINTESSENCE:
		m_sprite->loadTexture("Ball_black_OA.png");
	case Entity::NONE:
	default:
		break;
	}
	m_sprite->setHeight(m_radius * 2);
}

void Projectile::launch(const Vector3f& speed)
{
	m_movement->launch(speed);
}

void Projectile::tick(float deltaSeconds)
{
	Actor::tick(deltaSeconds);
	m_curLifetime += deltaSeconds;
	if (m_curLifetime > m_maxLifetime)
	{
		destroy();
	}
}