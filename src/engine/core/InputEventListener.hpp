#pragma once

#include <SFML/Window/Event.hpp>
#include <EASTL/vector.h>

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