#include "components/SpriteComponent.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "actors/Actor.hpp"
#include "core/AssetManager.hpp"

SpriteComponent::~SpriteComponent() {}

SpriteComponent::SpriteComponent() : m_sprite(new sf::Sprite()) {}

SpriteComponent::SpriteComponent(Actor* owner) : PrimitiveComponent(owner), m_sprite(new sf::Sprite()) {}

SpriteComponent::SpriteComponent(SceneComponent* parent) : PrimitiveComponent(parent), m_sprite(new sf::Sprite()) {}

void SpriteComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_sprite, states);
}

void SpriteComponent::loadTexture(const eastl::string& filename)
{
	if (!m_sprite)
	{
		m_sprite.reset(new sf::Sprite());
	}
	m_texture = AssetManager::loadTexture(filename);
	m_sprite->setTexture(*m_texture);
	sf::Vector2u textureSize = m_texture->getSize();
	m_sprite->setOrigin(textureSize.x / 2, textureSize.y * (1 - INVSQRT_3));
}

void SpriteComponent::setHeight(float value)
{
	EASTL_ASSERT(value != 0 && m_texture);
	uint32_t textureHeight = m_texture->getSize().y;
	float ratio = SQRT_3 * value / textureHeight;
	m_sprite->setScale(ratio, ratio);
}

void SpriteComponent::setTexture(sf::Texture* texture)
{
	m_texture = texture;
	m_sprite->setTexture(*texture);
	sf::Vector2u textureSize = m_texture->getSize();
	m_sprite->setOrigin(textureSize.x / 2, textureSize.y * (1 - INVSQRT_3));
}