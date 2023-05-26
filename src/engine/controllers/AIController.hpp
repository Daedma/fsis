#pragma once

#include "controllers/Controller.hpp"

class AIController : public Controller
{
	Character* m_target = nullptr;

	ActionID m_attack;

	float m_attackRange = 0.f;

public:
	void setTarget(Character* target)
	{
		m_target = target;
	}

	void bindAttackAction(ActionID id)
	{
		m_attack = id;
	}

	void setAttackRange(float range)
	{
		m_attackRange = range;
	}

	virtual void tick(float deltaSeconds) override;
};