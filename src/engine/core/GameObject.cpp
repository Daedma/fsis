#include "core/GameObject.hpp"
#include "core/World.hpp"

GameObject::GameObject(World* world) :
	m_world(world)
{
	m_world->registry(this);
}

GameObject::~GameObject()
{
	m_world->unregistry(this);
}

void GameObject::tick(float deltaSeconds) {}

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
	if (m_world)
	{
		m_world->unregistry(this);
		m_tickgroup = tickGroup;
		m_world->registry(this);
	}
	else
	{
		m_tickgroup = tickGroup;
	}
}

void GameObject::onDestroyed() {}

void GameObject::destroy()
{
	onDestroyed();
}

void GameObject::initWorld(World* world)
{
	if (!m_world)
	{
		m_world = world;
		m_world->registry(this);
	}
}