#pragma once

#include "State.hpp"
#include "MatchStats.hpp"
#include "Achievement.hpp"
#include <vector>
#include <atomic>

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