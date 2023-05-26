#include "FSISCharacter.hpp"
#include "core/AssetManager.hpp"
#include "core/World.hpp"
#include "Components.hpp"
#include "controllers/Controller.hpp"
#include <fstream>
#include <EASTL/algorithm.h>

FSISCharacter::FSISCharacter(World* world) : Character(world)
{
	m_proxBox = new BoxComponent();
	m_proxBox->attachToActor(this);

	m_targetPosition = m_nearestCharacters.end();
}

void FSISCharacter::load(const eastl::string& filename)
{
	namespace json = boost::json;

	std::ifstream ifs(filename.c_str());
	EASTL_ASSERT(ifs);

	json::object characterInfo = json::parse(ifs).as_object();
	setMaxHP(characterInfo.at("hp").as_double());

	setAttackSpeed(characterInfo.at("attackSpeed").as_double());

	if (m_anim)
	{
		m_anim->destroy();
	}
	m_anim = AssetManager::loadCharacterAnimation(characterInfo.at("sprite").as_string().c_str());
	EASTL_ASSERT(m_anim->checkInitializationCompleteness());
	m_anim->setInitState();

	setSize({
		characterInfo.at("width").as_double(),
		characterInfo.at("height").as_double()
		}
	);

	setDamage(characterInfo.at("damage").as_double());

	getMovementComponent()->setWalkSpeed(characterInfo.at("defaultSpeed").as_double());
	getMovementComponent()->setRunSpeed(characterInfo.at("runSpeed").as_double());

	load_Internal(characterInfo);
}

void FSISCharacter::setSize(const Vector2f& size)
{
	m_proxBox->setSize({ size.x * INVSQRT_2, size.x * INVSQRT_2, size.y });
	if (m_anim)
	{
		m_anim->setHeight(size.y);
	}
}

void FSISCharacter::setTarget(FSISCharacter* target)
{
	if (target)
	{
		m_target->onEndTargeted(this);
		m_target = target;
		m_targetPosition = eastl::find(m_nearestCharacters.begin(), m_nearestCharacters.end(), target);
		getMovementComponent()->setOrientRotationToMovement(false);
		target->m_inTarget = true;
		target->onBeginTargeted(this);
	}
	else
	{
		if (m_target)
		{
			m_target->m_inTarget = false;
			m_target->onEndTargeted(this);
		}
		m_target = nullptr;
		m_targetPosition = m_nearestCharacters.end();
		getMovementComponent()->setOrientRotationToMovement(true);
	}
}

void FSISCharacter::nextTarget()
{
	updateNearestCharacters();
	if (m_target)
	{
		if (m_targetPosition == m_nearestCharacters.end())
		{
			m_targetPosition = m_nearestCharacters.begin();
		}
		else
		{
			++m_targetPosition;
		}
		for (; m_targetPosition != m_nearestCharacters.end(); ++m_targetPosition)
		{
			if (mathter::Distance((*m_targetPosition)->getPosition(), getPosition()) <= m_attackRange)
			{
				setTarget(dynamic_cast<FSISCharacter*>(*m_targetPosition));
				return;
			}
		}
		setTarget(nullptr);
	}
}

void FSISCharacter::takeDamage(float damage, FSISCharacter* instigator)
{
	if (!isAlive())
	{
		return;
	}
	m_health = eastl::clamp<float>(m_health - damage, 0, m_maxHealth);
	if (m_health == 0.f)
	{
		onDeath(instigator);
	}
}

void FSISCharacter::attack()
{
	m_timeSinceLastAttack = 0.f;
}

void FSISCharacter::tick(float deltaSeconds)
{
	updateCurrentTarget();
	if (m_target)
	{
		Vector3f direction = m_target->getPosition() - getPosition();
		Vector3f orientation(direction.x, direction.y, 0);
		orientByDirection(mathter::Normalize(orientation));
	}
	m_timeSinceLastAttack += deltaSeconds;
}

void FSISCharacter::onDeath(FSISCharacter* killer)
{
	if (m_target)
	{
		resetTarget();
	}
	killer->onKill(this);
	getMovementComponent()->deactivate();
}

void FSISCharacter::onKill(FSISCharacter* victim) {}

void FSISCharacter::onBeginTargeted(FSISCharacter* killer) {}

void FSISCharacter::onEndTargeted(FSISCharacter* hunter) {}

void FSISCharacter::load_Internal(const boost::json::object& obj) {}

void FSISCharacter::updateNearestCharacters()
{
	if ((getWorld()->getControllers().size() - 1) != m_nearestCharactersCount)
	{
		eastl::vector<Character*> nearest = getController()->getNearestCharacters();
		m_nearestCharacters.clear();
		eastl::copy(nearest.begin(), nearest.end(), eastl::back_inserter(m_nearestCharacters));
		m_nearestCharactersCount = nearest.size();
		m_targetPosition = m_nearestCharacters.end();
	}
	m_nearestCharacters.sort([this](Character* lhs, Character* rhs) {
		return mathter::LengthSquared(getPosition() - lhs->getPosition()) <
			mathter::LengthSquared(getPosition() - rhs->getPosition());
		});
}

void FSISCharacter::updateCurrentTarget()
{
	if (m_autoResetTarget && m_target && mathter::Distance(getPosition(), m_target->getPosition()) > m_attackRange)
	{
		setTarget(nullptr);
	}
}