#include "components/SpriteComponent.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "actors/Actor.hpp"

SpriteComponent::~SpriteComponent() {}

SpriteComponent::SpriteComponent() : m_sprite(new sf::Sprite()) {}

SpriteComponent::SpriteComponent(Actor* owner) : PrimitiveComponent(owner), m_sprite(new sf::Sprite()) {}

SpriteComponent::SpriteComponent(SceneComponent* parent) : PrimitiveComponent(parent), m_sprite(new sf::Sprite()) {}

void SpriteComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_sprite, states);
}

void SpriteComponent::updateTexture(sf::Texture* texture)
{
	m_texture.reset(texture);
}

void SpriteComponent::loadTexture(eastl::string_view filename)
{
	if (!m_sprite)
	{
		m_sprite.reset(new sf::Sprite());
	}
	if (!m_texture)
	{
		m_texture.reset(new sf::Texture());
	}
	m_texture->loadFromFile(filename.data());
	m_sprite->setTexture(*m_texture);
	setOrigin(m_origin);
}

void SpriteComponent::setTexture(sf::Texture* texture)
{
	m_texture.reset(texture);
	m_sprite->setTexture(*m_texture);
	setOrigin(m_origin);
}

void SpriteComponent::setOrigin(uint8_t mask)
{
	float x = 0, y = 0;
	if (mask & OriginY::CENTER)
	{
		y = m_texture->getSize().y / 2;
	}
	else if (mask & OriginY::DOWN)
	{
		y = m_texture->getSize().y;
	}
	else if (mask & OriginY::UP)
	{
		y = 0;
	}
	if (mask & OriginX::CENTER)
	{
		x = m_texture->getSize().x / 2;
	}
	else if (mask & OriginX::LEFT)
	{
		x = 0;
	}
	else if (mask & OriginX::RIGHT)
	{
		x = m_texture->getSize().x;
	}
	m_origin = mask;
	m_sprite->setOrigin(x, y);
}

void SpriteComponent::setHeight(float value)
{
	EASTL_ASSERT(value != 0 && m_texture);
	uint32_t textureHeight = m_texture->getSize().y;
	// float ratio = textureHeight / (2 * value);
	float ratio = sqrt(3) * value / textureHeight;
	m_sprite->setScale(ratio, ratio);
}