/**
 * @file World.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of World class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
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

/**
 * @brief Container for all actors.
 * Provide game session service.
 */
class World
{
private:
	// Tick groups
	eastl::vector_set<GameObject*> m_prephys;
	eastl::vector_set<GameObject*> m_phys;
	eastl::vector_set<GameObject*> m_postphys;

	eastl::unique_ptr<GameMode> m_mode;

	eastl::unique_ptr<CollisionResolver> m_collision;

	eastl::vector<eastl::unique_ptr<Controller>> m_controllers;

	bool m_isFinished = false;

	float m_globalAcceleration = -500.f;

	struct ActorComponentPriorityCompare
	{
		bool operator()(ActorComponent* lhs, ActorComponent* rhs);
	};

	eastl::vector_set<Actor*> m_actorsToDestroy;
	eastl::vector_set<ActorComponent*, ActorComponentPriorityCompare>
		m_componentsToDestroy;

	Map* m_map = nullptr;

	eastl::vector<eastl::unique_ptr<Actor>> m_actors;

	bool m_paused = false;
public:
	~World();

	World();

	CollisionResolver* getCollisionResolver() const { return m_collision.get(); }

	void tick(float deltaSeconds);

	bool isFinished() const { return m_isFinished; }

	void start();

	void setPaused(bool paused)
	{
		m_paused = paused;
	}

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

	GameMode* getGameMode() const
	{
		return m_mode.get();
	}

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

	void removeDanglingControllers();

	/**
	 * @brief Get the gravitational acceleration of the world
	 *
	 * @return gravitational acceleration
	 */
	float getGravity() const { return m_globalAcceleration; }

private:
	void execDestroy();

	/**
	 * @brief Spawn actor in the world
	 *
	 * @param actor actor to spawned
	 *
	 * @note Not recommended, use template version instead
	 */
	void spawnActor(Actor* actor, const Vector3f& pos = ZERO_VECTOR);

	void spawnController(Controller* controller);

	void setGameMode(GameMode* mode);
};