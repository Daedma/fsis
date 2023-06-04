/**
 * @file AuthState.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of AuthState class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "State.hpp"
#include <atomic>

/**
 * @brief A class represents of authorization state of program.
 * From this state program can only switch to menu state.
 */
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