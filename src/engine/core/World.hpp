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
class Map;
class GameMode;

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

	Map* m_map = nullptr;

	eastl::unique_ptr<GameMode> m_mode;
public:
	~World();

	World();

	CollisionResolver* getCollisionResolver() const { return m_collision.get(); }

	void tick(float deltaSeconds);

	bool isFinished() const { return m_isFinished; }

	void start();

	void finish();

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

	Map* getMap() const { return m_map; }

	template<typename T>
	T* setGameMode()
	{
		T* mode = new T(this);
		setGameMode(mode);
		return mode;
	}

	template<typename T>
	T* spawnMap()
	{
		T* map = spawnActor<T>();
		m_map = map;
		return map;
	}

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

	const eastl::vector<eastl::unique_ptr<Controller>>& getControllers() const
	{
		return m_controllers;
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

	void setGameMode(GameMode* mode);
};