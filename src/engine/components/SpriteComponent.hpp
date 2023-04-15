#pragma once

#include "components/PrimitiveComponent.hpp"
#include <EASTL/unique_ptr.h>
#include <EASTL/string_view.h>

namespace sf
{
	class Sprite;
	class Texture;
}
class Actor;

namespace OriginY
{
	enum : uint8_t
	{
		UP = 0x1,
		CENTER = 0x2,
		DOWN = 0x4
	};
} // namespace OriginY


namespace OriginX
{

	enum : uint8_t
	{
		LEFT = 0x8,
		CENTER = 0x10,
		RIGHT = 0x20
	};
} // namespace OriginX

class SpriteComponent : public PrimitiveComponent
{
	eastl::unique_ptr<sf::Sprite> m_sprite;

	eastl::unique_ptr<sf::Texture> m_texture = nullptr;

public:

	static constexpr uint8_t ONFLOOR = OriginY::DOWN | OriginX::CENTER;

	static constexpr uint8_t FLYING = OriginY::CENTER | OriginX::CENTER;

	static constexpr uint8_t ONCEILING = OriginY::UP | OriginX::CENTER;

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

	void loadTexture(eastl::string_view filename);

	void setTexture(sf::Texture* texture);

	void setOrigin(uint8_t mask);

	uint8_t getOrigin() const { return m_origin; }

	//TODO make more fit texture usage interface

	sf::Sprite* getSprite() const { return m_sprite.get(); }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	uint8_t m_origin = ONFLOOR;
};