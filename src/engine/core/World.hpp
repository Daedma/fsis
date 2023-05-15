#pragma once
#include "EASTL/vector_set.h"
#include "EASTL/queue.h"
#include "EASTL/priority_queue.h"
#include "EASTL/unique_ptr.h"
#include "EASTL/vector.h"
#include <core/TransformTypes.hpp>

class GameObject;
class Actor;
class ActorComponent;
class Character;
class CollisionResolver;
class Controller;

class World
{
private:
	// Tick groups
	eastl::vector_set<GameObject*> m_prephys;
	eastl::vector_set<GameObject*> m_phys;
	eastl::vector_set<GameObject*> m_postphys;

	eastl::vector<eastl::unique_ptr<Controller>> m_controllers;

	eastl::vector<eastl::unique_ptr<Actor>> m_actors;

	eastl::unique_ptr<CollisionResolver> m_collision;

	bool m_isFinished = false;

	float m_globalAcceleration = -320.8f;

	struct ActorComponentPriorityCompare
	{
		bool operator()(ActorComponent* lhs, ActorComponent* rhs);
	};

	eastl::vector_set<Actor*> m_actorsToDestroy;
	eastl::vector_set<ActorComponent*, ActorComponentPriorityCompare>
		m_componentsToDestroy;

public:
	~World();

	World();

	CollisionResolver* getCollisionResolver() const { return m_collision.get(); }

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

	template<typename T>
	T* spawnActor(const Vector3f& pos = ZERO_VECTOR)
	{
		T* actor = new T(this);
		spawnActor(actor, pos);
		return actor;
	}

	template<typename T>
	T* spawnController()
	{
		T* controller = new T();
		spawnController(controller);
		return controller;
	}

	/**
	 * @brief Get the gravitational acceleration of the world
	 *
	 * @return gravitational acceleration
	 */
	float getGravity() const { return m_globalAcceleration; }

	// eastl::vector<Character*> getNearestCharacters(const Character* character);

private:
	void execDestroy();

	/**
	 * @brief Spawn actor in the world
	 *
	 * @param actor actor to spawned
	 *
	 * @note Not recommendet, use template version instead
	 */
	void spawnActor(Actor* actor, const Vector3f& pos = ZERO_VECTOR);

	void spawnController(Controller* controller);
};