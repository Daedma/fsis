#pragma once

#include "components/PrimitiveComponent.hpp"
#include <EASTL/unique_ptr.h>

namespace sf
{
	class Sprite;
	class Texture;
}
class Actor;

class SpriteComponent : public PrimitiveComponent
{
	eastl::unique_ptr<sf::Sprite> m_sprite;

	eastl::unique_ptr<sf::Texture> m_texture = nullptr;

public:
	SpriteComponent();

	SpriteComponent(Actor* owner);

	SpriteComponent(SceneComponent* parent);

	~SpriteComponent();

	/**
	 * @brief Save texture in this component.
	 * Use every time you change/set texture
	 * of sprite owned by this component
	 *
	 * @param texture new texture instance
	 */
	void updateTexture(sf::Texture* texture);

	//TODO make more fit texture usage interface

	sf::Sprite* getSprite() const { return m_sprite.get(); }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};