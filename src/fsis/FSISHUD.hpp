#pragma once 

#include "gui/HUD.hpp"

class FSISHUD : public HUD
{
public:
	FSISHUD(sf::RenderWindow* window) : HUD(window) {}

	enum class States
	{
		AUTH,
		MENU,
		CONTROL,
		EXIT,
		GAME,
		ESC,
		WAVE,
		TOTAL,
		STATESCOUNT
	};

	void setState(States state);

private:
	States m_curState = States::AUTH;
};