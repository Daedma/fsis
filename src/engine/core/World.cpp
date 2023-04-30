#include "core/World.hpp"
#include "components/ActorComponent.hpp"
#include "actors/Actor.hpp"
#include "core/GameObject.hpp"
#include "World.hpp"
#include "core/CollisionResolver.hpp"
#include "controllers/Controller.hpp"

World::~World() {}

World::World() : m_collision(new CollisionResolver()) {}

bool World::ActorComponentPriorityCompare::operator()(ActorComponent* lhs, ActorComponent* rhs)
{
	return lhs->getDepth() < rhs->getDepth();
}

void World::registry(GameObject* obj)
{
	switch (obj->getTickGroup())
	{
	case TickGroups::PREPHYSIC:
		m_prephys.emplace(obj);
		break;
	case TickGroups::PHYSIC:
		m_phys.emplace(obj);
		break;
	case TickGroups::POSTPHYSIC:
		m_postphys.emplace(obj);
		break;
	default:
		return;
	}
}

void World::unregistry(GameObject* obj)
{
	switch (obj->getTickGroup())
	{
	case TickGroups::PREPHYSIC:
		m_prephys.erase(obj);
		break;
	case TickGroups::PHYSIC:
		m_phys.erase(obj);
		break;
	case TickGroups::POSTPHYSIC:
		m_postphys.erase(obj);
		break;
	default:
		return;

	}
}

void World::execDestroy()
{
	while (!m_componentsToDestroy.empty())
	{
		m_componentsToDestroy.top()->forceDestroy();
		m_componentsToDestroy.pop();
	}
	while (!m_actorsToDestroy.empty())
	{
		Actor* actorToDestroy = m_actorsToDestroy.front();
		m_actors.erase(eastl::find_if(m_actors.begin(), m_actors.end(),
			[actorToDestroy](const eastl::unique_ptr<Actor>& rhs) {return rhs.get() == actorToDestroy;
			}));
		m_actorsToDestroy.pop();
	}
}

void World::spawnActor(Actor* actor, const Vector3f& pos)
{
	m_actors.emplace_back(actor);
	actor->setPosition(pos);
}

void World::spawnController(Controller* controller)
{
	m_controllers.emplace_back(controller);
}

void World::tick(float deltaSeconds)
{
	for (auto& i : m_controllers)
	{
		i->tick(deltaSeconds);
	}
	for (GameObject* i : m_prephys)
	{
		i->tick(deltaSeconds);
	}
	for (GameObject* i : m_phys)
	{
		i->tick(deltaSeconds);
	}
	m_collision->tick(deltaSeconds);
	for (GameObject* i : m_postphys)
	{
		i->tick(deltaSeconds);
	}
	execDestroy();
}
