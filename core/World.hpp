#pragma once
#include "EASTL/vector_set.h"
#include "EASTL/queue.h"
#include "EASTL/priority_queue.h"
#include "EASTL/unique_ptr.h"
#include "components/ActorComponent.hpp"

class GameObject;
class Actor;
class ActorComponent;

class World
{
private:
	// Tick groups
	eastl::vector_set<GameObject*> m_prephys;
	eastl::vector_set<GameObject*> m_phys;
	eastl::vector_set<GameObject*> m_postphys;

	eastl::vector<eastl::unique_ptr<Actor>> m_actors;

	bool m_isFinished = false;

	static constexpr auto m_comp = [](ActorComponent* lhs, ActorComponent* rhs) {return lhs->getDepth() < rhs->getDepth();};

	eastl::queue<Actor*> m_actorsToDestroy;
	eastl::priority_queue < ActorComponent*, eastl::vector<ActorComponent*>, decltype(m_comp)>
		m_componentsToDestroy;

public:
	void tick(float deltaSeconds);

	bool isFinished() const { return m_isFinished; }

	void finish() { m_isFinished = true; }

	/**
	 * @brief Add GameObject to tickgroup
	 *
	 * @param obj
	 */
	void registry(GameObject* obj);

	/**
	 * @brief Remove GameObject from tickgroup
	 *
	 * @param obj
	 * @note If @p obj is already unregistered
	 * this operation has no effect
	 */
	void unregistry(GameObject* obj);

	void destroyActor(Actor* actor) { m_actorsToDestroy.emplace(actor); }

	void destroyComponent(ActorComponent* component) { m_componentsToDestroy.emplace(component); }

	/**
	 * @brief Spawn actor in the world
	 *
	 * @param actor actor to spawned
	 */
	void spawnActor(Actor* actor); //TODO allow user to specify spawn position

private:
	void execDestroy();

};