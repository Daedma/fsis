#pragma once

#include <SFML/Graphics/Drawable.hpp>

class HUD: public sf::Drawable
{
public:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};