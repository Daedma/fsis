#include "components/PrimitiveComponent.hpp"
#include "render/Canvas.hpp"

PrimitiveComponent::PrimitiveComponent()
{
	setTickGroup(TickGroups::POSTPHYSIC);
	Canvas::registry(this);
}

PrimitiveComponent::~PrimitiveComponent()
{
	Canvas::unregistry(this);
}

PrimitiveComponent::PrimitiveComponent(Actor* owner) : SceneComponent(owner)
{
	setTickGroup(TickGroups::POSTPHYSIC);
	Canvas::registry(this);
}

PrimitiveComponent::PrimitiveComponent(SceneComponent* parent) : SceneComponent(parent)
{
	setTickGroup(TickGroups::POSTPHYSIC);
	Canvas::registry(this);
}

void PrimitiveComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const {}

void PrimitiveComponent::setLayer(int32_t layer)
{
	Canvas::unregistry(this);
	m_layer = layer;
	Canvas::registry(this);
}

void PrimitiveComponent::hide()
{
	Canvas::unregistry(this);
}