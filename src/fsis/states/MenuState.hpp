/**
 * @file MenuState.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of MenuState class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "State.hpp"

/**
 * @brief A class represents main menu state of program.
 * From this state program can finish or switch to game state
 */
class MenuState : public State
{
	MenuState() = default;

public:
	static MenuState* getInstance()
	{
		static MenuState instance;
		return &instance;
	}

	virtual void tick(float deltaSeconds) override;

	void init();

private:
	void toFight();

	void showControl();

};