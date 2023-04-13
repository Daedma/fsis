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
