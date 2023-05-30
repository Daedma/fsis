#pragma once

#include "states/State.hpp"

class ClientApplication
{
public:
	ClientApplication();

	void run();

private:
	class FSISHUD* m_hud;

	State* m_curState = nullptr;

};