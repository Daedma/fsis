#pragma once

#include "components/SceneComponent.hpp"
#include <SFML/Graphics/Drawable.hpp>

class PrimitiveComponent : public SceneComponent, public sf::Drawable
{
public:
	PrimitiveComponent();

	PrimitiveComponent(Actor* owner);

	PrimitiveComponent(SceneComponent* parent);

	virtual ~PrimitiveComponent();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};