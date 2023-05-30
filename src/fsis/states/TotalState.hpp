#pragma once

#include "State.hpp"

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
};