/**
 * @file State.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of State class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

/**
 * @brief Abstract base class for all program states.
 */
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