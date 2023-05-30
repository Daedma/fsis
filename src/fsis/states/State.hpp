#pragma once

class State
{
public:
	virtual void tick(float deltaSeconds) = 0;

	// https://stackoverflow.com/a/2109438
	State* nextState() const
	{
		return m_nextState;
	}

protected:
	State* m_nextState = this;

};