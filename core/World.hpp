#pragma once
#include "EASTL/vector_set.h"
#include "EASTL/queue.h"
#include "EASTL/priority_queue.h"
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

	static constexpr auto m_comp = [](ActorComponent* lhs, ActorComponent* rhs) {return lhs->getDepth() < rhs->getDepth();};

	eastl::queue<Actor*> m_actorsToDestroy;
	eastl::priority_queue < ActorComponent*, eastl::vector<ActorComponent*>, decltype(m_comp)>
		m_componentsToDestroy;

public:
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

private:
	void execDestroy();

};