#include "gui/HUD.hpp"
#include <SFML/Graphics/RenderStates.hpp>

void HUD::processInput(const sf::Event& event)
{
	tgui::GuiSFML::handleEvent(event);
}