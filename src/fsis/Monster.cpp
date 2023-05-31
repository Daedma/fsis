#include "Monster.hpp"
#include "FSISAction.hpp"
#include "Projectile.hpp"
#include "Core.hpp"
#include "Components.hpp"
#include "controllers/Controller.hpp"

Monster::Monster(World* world) : FSISCharacter(world)
{
	setAutoResetTarget(false);
	m_marker = new SpriteComponent();
	m_marker->loadTexture("target3.png");
	m_marker->setLayer(2);
	m_marker->setHeight(m_proxBox->getSize().z);
	m_marker->hide();
	m_marker->attachToActor(this);

	m_sound = new AudioComponent();
	m_sound->attachToActor(this);
}

void Monster::setupController(Controller* controller)
{
	controller->bindAction(FSISActions::ATTACK, Controller::InputEvent::PRESSED, this, &Monster::attack);
}

void Monster::attack()
{
	if (canAttack())
	{
		Projectile* ball = getWorld()->spawnActor<Projectile>();
		ball->setRadius(25);
		initProjectilePosition(ball);
		ball->setType(getEntity());
		ball->setOnHit([this](FSISCharacter* target, Entity) {
			if (target == getTarget())
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

void Monster::onBeginTargeted(FSISCharacter* hunter)
{
	m_marker->show();
}

void Monster::onEndTargeted(FSISCharacter* hunter)
{
	m_marker->hide();
}

void Monster::load_Internal(const boost::json::object& info)
{
	// TODO add aditional parameters
}

void Monster::takeDamage(float value, FSISCharacter* instigator)
{
	FSISCharacter::takeDamage(value, instigator);
	m_sound->playSound("tank_death_5.wav");
}

void Monster::onDeath(FSISCharacter* killer)
{
	FSISCharacter::onDeath(killer);
	m_sound->playSound("tank_death_2.wav");
}