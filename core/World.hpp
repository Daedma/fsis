#pragma once
#include "EASTL/vector_set.h"
#include "EASTL/queue.h"

class GameObject;
class Actor;

class World
{
private:
	// Tick groups
	eastl::vector_set<GameObject*> m_prephys;
	eastl::vector_set<GameObject*> m_phys;
	eastl::vector_set<GameObject*> m_postphys;

	eastl::queue<Actor*> m_actorsToDestroy;
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
};