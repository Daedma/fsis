#pragma once

#include "FSISCharacter.hpp"

class Sorcerer : public FSISCharacter
{
public:
	Sorcerer(World* world) : FSISCharacter(world) {}

	void moveForward(float value);

	void moveRight(float value);

	void specialAttack();

	void specialMode();

	void specialAbility();

	void toggleRun();

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
};