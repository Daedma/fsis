#pragma once

#include "State.hpp"
#include <atomic>

class AuthState : public State
{
	AuthState() = default;
public:

	static AuthState* getInstance()
	{
		static AuthState instance;
		return &instance;
	}

	void init();

	virtual void tick(float deltaSeconds) override;

private:

	std::atomic_bool m_responseReceived = false;

	bool m_successfulLogin = false;

private:

	void tryToLogin();
};