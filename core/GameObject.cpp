#include "core/GameObject.hpp"
#include "core/World.hpp"

GameObject::GameObject(World* world):
	m_world(world)
{
	m_world->registry(this);
}

void GameObject::tick(float deltaSeconds)
{

}

void GameObject::deactivate()
{
	m_world->unregistry(this);
}

void GameObject::activate()
{
	m_world->registry(this);
}

void GameObject::setTickGroup(TickGroups tickGroup)
{
	m_world->unregistry(this);
	m_tickgroup = tickGroup;
	m_world->registry(this);
}