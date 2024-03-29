#include "core/World.hpp"
#include "components/ActorComponent.hpp"
#include "actors/Actor.hpp"
#include "core/GameObject.hpp"
#include "World.hpp"
#include "core/CollisionResolver.hpp"
#include "controllers/Controller.hpp"
#include "core/GameMode.hpp"
#include <EASTL/algorithm.h>

World::~World() {}

World::World() : m_collision(new CollisionResolver()), m_mode(nullptr) {}

bool World::ActorComponentPriorityCompare::operator()(ActorComponent* lhs, ActorComponent* rhs)
{
	return lhs->getDepth() < rhs->getDepth();
}

void World::start()
{
	m_mode->spawnPlayer();
	m_mode->onStartLevel();
}

void World::finish()
{
	m_isFinished = true;
	m_mode->onEndLevel();
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
	for (ActorComponent* i : m_componentsToDestroy)
	{
		i->forceDestroy();
	}
	m_componentsToDestroy.clear();
	for (Actor* i : m_actorsToDestroy)
	{
		m_actors.erase(eastl::find_if(m_actors.begin(), m_actors.end(),
			[i](const eastl::unique_ptr<Actor>& rhs) {return rhs.get() == i;
			}));
	}
	m_actorsToDestroy.clear();
}

void World::setGameMode(GameMode* mode)
{
	m_mode.reset(mode);
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

void World::removeDanglingControllers()
{
	eastl::erase_if(m_controllers,
		[](const eastl::unique_ptr<Controller>& controller) {
			return !controller->getMarionette();
		});
}

void World::tick(float deltaSeconds)
{
	if (m_paused)
	{
		return;
	}
	for (auto& i : m_controllers)
	{
		i->tick(deltaSeconds);
	}
	for (GameObject* i : m_prephys)
	{
		i->tick(deltaSeconds);
	}
	m_collision->updateAABB();
	for (GameObject* i : m_phys)
	{
		i->tick(deltaSeconds);
	}
	m_collision->tick(deltaSeconds);
	for (GameObject* i : m_postphys)
	{
		i->tick(deltaSeconds);
	}
	m_mode->tick(deltaSeconds);
	execDestroy();
}
