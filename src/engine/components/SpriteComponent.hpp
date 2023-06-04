/**
 * @file SpriteComponent.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains declaration of SpriteComponent class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
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

/**
 * @brief Provide sprite support for actors.
 */
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

	sf::Sprite* getSprite() const { return m_sprite.get(); }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setHeight(float height);

};