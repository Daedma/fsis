#pragma once

#include "components/PrimitiveComponent.hpp"
#include <EASTL/unique_ptr.h>
#include <EASTL/string.h>

namespace sf
{
	class Sprite;
	class Texture;
}
class Actor;

class SpriteComponent : public PrimitiveComponent
{
	eastl::unique_ptr<sf::Sprite> m_sprite;

	sf::Texture* m_texture = nullptr;

public:

	SpriteComponent();

	SpriteComponent(Actor* owner);

	SpriteComponent(SceneComponent* parent);

	~SpriteComponent();

	void setTexture(sf::Texture* texture);

	void loadTexture(const eastl::string& filename);

	//TODO make more fit texture usage interface

	sf::Sprite* getSprite() const { return m_sprite.get(); }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setHeight(float height);

};