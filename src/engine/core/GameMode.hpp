/**
 * @file GameMode.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of GameMode class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

class World;

/**
 * @brief A class represents game rules.
 */
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