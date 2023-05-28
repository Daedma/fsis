#pragma once

class World;

class GameMode
{
	World* m_world;

public:
	GameMode(World* world) : m_world(world) {}

	World* getWorld() const
	{
		return m_world;
	}

	virtual void spawnPlayer();

	virtual void onStartLevel();

	virtual void onEndLevel();

	virtual void tick(float deltaSeconds);
};