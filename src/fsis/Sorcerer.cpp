#include "Sorcerer.hpp"
#include "Components.hpp"
#include "Core.hpp"
#include "controllers/Controller.hpp"
#include "Projectile.hpp"
#include "FSISAction.hpp"

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
	if (canAttack())
	{
		Projectile* ball = getWorld()->spawnActor<Projectile>();
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
	}
}

void Sorcerer::specialMode()
{
	//TODO ask gamedesigner
}

void Sorcerer::specialAbility()
{
	if (m_beacon.ptr)
	{
		setPosition(m_beacon.ptr->getPosition());
	}
	else
	{
		m_beacon.ptr = getWorld()->spawnActor<Actor>(getPosition());
		SpriteComponent* beaconSprite = new SpriteComponent();
		beaconSprite->loadTexture("beacon.png");
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
	controller->bindAxis(FSISActions::MOVE_FORWARD, this, &Sorcerer::moveForward);
	controller->bindAxis(FSISActions::MOVE_RIGHT, this, &Sorcerer::moveRight);
}

void Sorcerer::attack()
{
	if (canAttack())
	{
		Projectile* ball = getWorld()->spawnActor<Projectile>();
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
	//TODO integration with game mode
}

void Sorcerer::onDeath(FSISCharacter* killer)
{
	FSISCharacter::onDeath(killer);
	//TODO integration with gamemode
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
}

void Sorcerer::load_Internal(const boost::json::object& obj)
{

}