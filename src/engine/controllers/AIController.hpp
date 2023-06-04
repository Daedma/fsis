/**
 * @file AIController.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of AIController
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "controllers/Controller.hpp"

/**
 * @brief A class representing an AI controller for a character.
 * This class inherits from the Controller class and provides functionality for controlling
 * an AI character.
 */
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