#pragma once

#include <SFML/Window/Event.hpp>

class InputEventListener
{
public:
	virtual void processInput(const sf::Event& event) = 0;
};