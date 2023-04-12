#include "components/SpriteComponent.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "actors/Actor.hpp"

SpriteComponent::SpriteComponent(): m_sprite(new sf::Sprite()) {}

SpriteComponent::SpriteComponent(Actor* owner): PrimitiveComponent(owner), m_sprite(new sf::Sprite()) {}

SpriteComponent::SpriteComponent(SceneComponent* parent): PrimitiveComponent(parent), m_sprite(new sf::Sprite()) {}

void SpriteComponent::setSprite(sf::Sprite* sprite)
{
	m_sprite.reset(sprite);
}

void SpriteComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_sprite, states);
}
