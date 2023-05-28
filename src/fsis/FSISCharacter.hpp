#pragma once

#include "actors/Character.hpp"
#include "Entity.hpp"
#include <boost/json.hpp>
#include <EASTL/string.h>
#include <EASTL/list.h>

class FSISCharacter : public Character
{
protected:

	FSISCharacter* m_target = nullptr;

	class CharacterAnimComponent* m_anim = nullptr;

	class BoxComponent* m_proxBox = nullptr;

	float m_timeSinceLastAttack = 0.f;

	float m_attackSpeed = 0.5f;

	float m_maxHealth = 100.f;

	float m_health = 100.f;

	float m_damage = 20.f;

	float m_attackRange = 100.f;

	bool m_inTarget = false;

	bool m_autoResetTarget = true;
public:
	FSISCharacter(World* world);

	void load(const eastl::string& filename);

	void setAutoResetTarget(bool autoResetTarget)
	{
		m_autoResetTarget = autoResetTarget;
	}

	bool shouldAutoResetTarget() const
	{
		return m_autoResetTarget;
	}

	void setAttackRange(float attackRange)
	{
		m_attackRange = attackRange;
	}

	float getAttackRange() const
	{
		return m_attackRange;
	}

	void setDamage(float damage)
	{
		m_damage = damage;
	}

	float getDamage() const
	{
		return m_damage;
	}

	void setAttackSpeed(float attackSpeed)
	{
		m_attackSpeed = attackSpeed;
	}

	void setMaxHP(float hp)
	{
		m_health = m_maxHealth = hp;
	}

	float getMaxHP() const
	{
		return m_maxHealth;
	}

	float getHP() const
	{
		return m_health;
	}

	float restoreHP()
	{
		float delta = m_maxHealth - m_health;
		m_health = m_maxHealth;
		return delta;
	}

	bool isAlive() const
	{
		return m_health > 0;
	}

	bool canAttack() const
	{
		return m_timeSinceLastAttack >= m_attackSpeed;
	}

	void setSize(const Vector2f& size);

	FSISCharacter* getTarget() const
	{
		return m_target;
	}

	void setTarget(FSISCharacter* target);

	void nextTarget();

	void resetTarget()
	{
		setTarget(nullptr);
	}

	void setEntity(Entity entity)
	{
		m_entity = entity;
	}

	Entity getEntity() const
	{
		return m_entity;
	}

	void kill(FSISCharacter* killer)
	{
		takeDamage(m_health, killer);
	}

	virtual void takeDamage(float damage, FSISCharacter* instigator);

	virtual void attack();

	virtual void tick(float deltaSeconds) override;

protected:
	virtual void onKill(FSISCharacter* victim);

	virtual void onDeath(FSISCharacter* killer);

	virtual void onBeginTargeted(FSISCharacter* hunter);

	virtual void onEndTargeted(FSISCharacter* hunter);

	virtual void load_Internal(const boost::json::object& obj);

	void initProjectilePosition(class Projectile* ball);

private:
	eastl::list<Character*> m_nearestCharacters;

	size_t m_nearestCharactersCount = 0;

	eastl::list<Character*>::iterator m_targetPosition;

	Entity m_entity = Entity::NONE;

private:
	void updateNearestCharacters();

	void updateCurrentTarget();
};