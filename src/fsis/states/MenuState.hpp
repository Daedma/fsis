#pragma once

#include "State.hpp"

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