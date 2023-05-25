#pragma once

#include <SFML/Window/Event.hpp>
#include <EASTL/vector.h>

class InputEventListener
{
	// static eastl::vector<InputEventListener*> m_listeners;
public:
	virtual void processInput(const sf::Event& event) = 0;
};