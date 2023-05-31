#pragma once

#include "FSISCharacter.hpp"
#include "components/AudioComponent.hpp"

class Sorcerer : public FSISCharacter
{
public:
	Sorcerer(World* world) : FSISCharacter(world)
	{
		m_sound = new AudioComponent();
		m_sound->attachToActor(this);
	}

	void moveForward(float value);

	void moveRight(float value);

	void specialAttack();

	void specialMode();

	void specialAbility();

	void changeEntity()
	{
		setEntity(m_curZone);
	}

	void setEntityZone(Entity zone)
	{
		m_curZone = zone;
	}

	void toggleRun();

	float getSpecialAttackCooldown() const
	{
		return eastl::clamp(m_specialAttack.timeSinceLastUse / m_specialAttack.cooldown, 0.f, 1.f);
	}

	float getSpecialModeCooldown() const
	{
		return eastl::clamp(m_specialMode.timeSinceLastUse / m_specialMode.cooldown, 0.f, 1.f);
	}

	float getSpecialAbilityCooldown() const
	{
		return eastl::clamp(m_beacon.timeSinceLastUse / m_beacon.cooldown, 0.f, 1.f);
	}

	virtual void setupController(Controller* controller) override;

	virtual void attack() override;

	virtual void tick(float deltaSeconds) override;

	virtual void takeDamage(float value, FSISCharacter* instigator) override;

protected:
	virtual void onKill(FSISCharacter* victim) override;

	virtual void onDeath(FSISCharacter* killer) override;

	virtual void load_Internal(const boost::json::object& obj) override;

private:
	struct
	{
		Actor* ptr = nullptr;
		float timer = 0.f;
		float duration = 20.f;
		float cooldown = 40.f;
		float timeSinceLastUse = 0;
	} m_beacon;

	struct
	{
		float radius;
		float cooldown;
		float speed;
		float timeSinceLastUse = 0;
	} m_specialAttack;

	struct
	{
		float cooldown;
		float proc;
		float duration = 0;
		bool active = false;
		float timeSinceLastUse = 0;
	} m_specialMode;

	Entity m_curZone = Entity::INFERNAL;

	AudioComponent* m_sound = nullptr;
};