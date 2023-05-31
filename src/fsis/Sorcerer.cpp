#include "Sorcerer.hpp"
#include "Components.hpp"
#include "Core.hpp"
#include "controllers/Controller.hpp"
#include "Projectile.hpp"
#include "FSISAction.hpp"
#include "FSISGameMode.hpp"
#include <random>

void Sorcerer::moveForward(float value)
{
	getMovementComponent()->move(-TO_PLAYER * value);
}

void Sorcerer::moveRight(float value)
{
	getMovementComponent()->move(TO_RIGHTSIDE * value);
}

void Sorcerer::specialAttack()
{
	if (canAttack() && m_specialAttack.timeSinceLastUse >= m_specialAttack.cooldown)
	{
		Projectile* ball = getWorld()->spawnActor<Projectile>();
		ball->setRadius(m_specialAttack.radius);
		ball->setSpeed(m_specialAttack.speed);
		initProjectilePosition(ball);
		ball->setType(Entity::QUINTESSENCE);
		ball->setOnHit([this](FSISCharacter* target, Entity) {
			target->kill(this);
			});
		if (m_target)
		{
			ball->launch(mathter::Normalize(m_target->getPosition() - getPosition()));
		}
		else
		{
			ball->launch(getForwardVector());
		}
		m_anim->playAnimation("spell");
		m_timeSinceLastAttack = 0;
		m_specialAttack.timeSinceLastUse = 0;
	}
}

void Sorcerer::specialMode()
{
	if (m_specialMode.timeSinceLastUse >= m_specialMode.cooldown)
	{
		m_specialMode.active = true;
		m_specialMode.timeSinceLastUse = 0;
	}
}

void Sorcerer::specialAbility()
{
	if (m_beacon.ptr)
	{
		setPosition(m_beacon.ptr->getPosition());
	}
	else if (m_beacon.timeSinceLastUse >= m_beacon.cooldown)
	{
		m_beacon.ptr = getWorld()->spawnActor<Actor>(getPosition());
		SpriteComponent* beaconSprite = new SpriteComponent();
		beaconSprite->loadTexture("beacon1.png");
		beaconSprite->setHeight(30);
		beaconSprite->attachToActor(m_beacon.ptr);
	}
}

void Sorcerer::toggleRun()
{
	getMovementComponent()->toggleRun();
}

void Sorcerer::setupController(Controller* controller)
{
	controller->bindAction(FSISActions::NEXT_TARGET, Controller::InputEvent::PRESSED, static_cast<FSISCharacter* const>(this), &FSISCharacter::nextTarget);
	controller->bindAction(FSISActions::ATTACK, Controller::InputEvent::PRESSED, this, &Sorcerer::attack);
	controller->bindAction(FSISActions::SP_ABILITY, Controller::InputEvent::PRESSED, this, &Sorcerer::specialAbility);
	controller->bindAction(FSISActions::SP_ATTACK, Controller::InputEvent::PRESSED, this, &Sorcerer::specialAttack);
	controller->bindAction(FSISActions::SP_MODE, Controller::InputEvent::PRESSED, this, &Sorcerer::specialMode);
	controller->bindAction(FSISActions::TOGGLE_RUN, Controller::InputEvent::PRESSED, this, &Sorcerer::toggleRun);
	controller->bindAction(FSISActions::TOGGLE_RUN, Controller::InputEvent::RELEASED, this, &Sorcerer::toggleRun);
	controller->bindAction(FSISActions::CHANGE_ENTITY, Controller::InputEvent::PRESSED, this, &Sorcerer::changeEntity);
	controller->bindAxis(FSISActions::MOVE_FORWARD, this, &Sorcerer::moveForward);
	controller->bindAxis(FSISActions::MOVE_RIGHT, this, &Sorcerer::moveRight);
}

void Sorcerer::attack()
{
	if (canAttack())
	{
		Projectile* ball = getWorld()->spawnActor<Projectile>();
		ball->setRadius(15);
		initProjectilePosition(ball);
		ball->setType(getEntity());
		ball->setOnHit([this](FSISCharacter* target, Entity entity) {
			if (target->getEntity() == entity)
			{
				target->takeDamage(getDamage(), this);
			}
			});
		if (m_target)
		{
			ball->launch(mathter::Normalize(m_target->getPosition() - getPosition()));
		}
		else
		{
			ball->launch(getForwardVector());
		}
		m_anim->playAnimation("spell");
		m_timeSinceLastAttack = 0;
	}
}

void Sorcerer::onKill(FSISCharacter* victim)
{
	FSISCharacter::onKill(victim);
	FSISGameMode* mode = dynamic_cast<FSISGameMode*>(getWorld()->getGameMode());
	if (mode)
	{
		mode->notifyMobKilledByPlayer();
	}
}

void Sorcerer::onDeath(FSISCharacter* killer)
{
	FSISCharacter::onDeath(killer);
	FSISGameMode* mode = dynamic_cast<FSISGameMode*>(getWorld()->getGameMode());
	if (mode)
	{
		mode->notifyPlayerDeath();
	}
}

void Sorcerer::tick(float deltaSeconds)
{
	FSISCharacter::tick(deltaSeconds);
	if (m_beacon.ptr)
	{
		m_beacon.timer += deltaSeconds;
		if (m_beacon.timer > m_beacon.duration)
		{
			m_beacon.ptr->destroy();
			m_beacon.ptr = nullptr;
			m_beacon.timer = 0;
		}
	}
	m_beacon.timeSinceLastUse += deltaSeconds;
	m_specialAttack.timeSinceLastUse += deltaSeconds;
	m_specialMode.timeSinceLastUse += deltaSeconds;
	m_specialMode.active = m_specialMode.timeSinceLastUse < m_specialMode.duration;
}

void Sorcerer::load_Internal(const boost::json::object& obj)
{
	namespace json = boost::json;

	json::object specialAttackInfo = obj.at("specialAttack").as_object();
	m_specialAttack.cooldown = specialAttackInfo.at("cooldown").as_double();
	m_specialAttack.radius = specialAttackInfo.at("radius").as_double();
	m_specialAttack.speed = specialAttackInfo.at("speed").as_double();
	m_specialAttack.timeSinceLastUse = m_specialAttack.cooldown;

	json::object specialModeInfo = obj.at("specialMode").as_object();
	m_specialMode.cooldown = specialModeInfo.at("cooldown").as_double();
	m_specialMode.proc = specialModeInfo.at("proc").as_double();
	m_specialMode.duration = specialModeInfo.at("duration").as_double();
	m_specialAttack.timeSinceLastUse = m_specialAttack.cooldown;

	json::object specialAbilityInfo = obj.at("specialAbility").as_object();
	m_beacon.duration = specialAbilityInfo.at("duration").as_double();
	m_beacon.cooldown = specialAbilityInfo.at("cooldown").as_double();
	m_beacon.timeSinceLastUse = m_beacon.cooldown;
}

void Sorcerer::takeDamage(float value, FSISCharacter* instigator)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::bernoulli_distribution d(1.f - m_specialMode.proc);
	if (m_specialMode.active && d(gen) || !m_specialMode.active)
	{
		FSISCharacter::takeDamage(value, instigator);
	}

}