#pragma once

#include "components/PrimitiveComponent.hpp"
#include <EASTL/unique_ptr.h>

namespace sf
{
	class Sprite;
}
class Actor;

class SpriteComponent: public PrimitiveComponent
{
	eastl::unique_ptr<sf::Sprite> m_sprite;

public:
	SpriteComponent();

	SpriteComponent(Actor* owner);

	SpriteComponent(SceneComponent* parent);

	void setSprite(sf::Sprite* sprite);

	sf::Sprite* getSprite() const { return m_sprite.get(); }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};