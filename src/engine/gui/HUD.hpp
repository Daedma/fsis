#pragma once

#include <TGUI/Backends/SFML.hpp>
#include <core/InputEventListener.hpp>

class HUD : public tgui::GuiSFML, public InputEventListener
{
public:
	HUD(sf::RenderWindow* window) : tgui::GuiSFML(*window) {}

	virtual void processInput(const sf::Event& event) override;
};