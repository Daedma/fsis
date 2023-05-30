#pragma once

#include "states/State.hpp"

class ClientApplication
{
public:
	ClientApplication();

	void run();

private:
	State* m_curState = nullptr;
};