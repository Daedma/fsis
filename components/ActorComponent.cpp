#include "components/ActorComponent.hpp"
#include "actors/Actor.hpp"

void ActorComponent::destroy()
{
	GameObject::destroy();
	if (m_owner)
	{
		m_owner->destroyComponent(this);
	}
}