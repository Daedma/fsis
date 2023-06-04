/**
 * @file TotalState.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of TotalState class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "State.hpp"
#include "MatchStats.hpp"
#include "Achievement.hpp"
#include <vector>
#include <atomic>

/**
 * @brief A class represents after match state of program.
 * From this state program can switch only to MenuState
 */
class TotalState : public State
{
	TotalState() = default;

public:
	static TotalState* getInstance()
	{
		static TotalState instance;
		return &instance;
	}

	virtual void tick(float deltaSeconds) override;

	void init(const class MatchStats& stats);

private:
	MatchStats m_stats;

	std::atomic_bool m_dataReceived = false;

	std::vector<Achievement> m_achievements;

	bool m_dataUpdated = false;

	std::atomic_bool m_error = false;

private:
	void sendStats();
};