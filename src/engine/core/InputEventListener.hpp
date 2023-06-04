/**
 * @file InputEventListener.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of InputEventListener class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SFML/Window/Event.hpp>
#include <EASTL/vector.h>

/**
 * @brief Interface for all classes are needed to be notified
 * of input events, such as key pressed etc.
 */
class InputEventListener
{
	static eastl::vector<InputEventListener*> m_listeners;

public:
	InputEventListener()
	{
		m_listeners.push_back(this);
	}

	~InputEventListener()
	{
		m_listeners.erase_first(this);
	}

	static void handleEvent(const sf::Event& event)
	{
		for (auto i : m_listeners)
		{
			i->processInput(event);
		}
	}

protected:

	virtual void processInput(const sf::Event& event) = 0;
};