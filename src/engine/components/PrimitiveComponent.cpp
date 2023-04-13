#include "components/PrimitiveComponent.hpp"
#include "render/Canvas.hpp"

PrimitiveComponent::PrimitiveComponent()
{
	setTickGroup(TickGroups::POSTPHYSIC);
	Canvas::getInstance()->registry(this);
}

PrimitiveComponent::~PrimitiveComponent()
{
	Canvas::getInstance()->unregistry(this);
}

PrimitiveComponent::PrimitiveComponent(Actor* owner) : SceneComponent(owner)
{
	setTickGroup(TickGroups::POSTPHYSIC);
	Canvas::getInstance()->registry(this);
}

PrimitiveComponent::PrimitiveComponent(SceneComponent* parent) : SceneComponent(parent)
{
	setTickGroup(TickGroups::POSTPHYSIC);
	Canvas::getInstance()->registry(this);
}

void PrimitiveComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const {}