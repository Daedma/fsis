#pragma once

#include "core/GameMode.hpp"
#include "MatchStats.hpp"
#include "Entity.hpp"
#include <EASTL/vector.h>

class FSISGameMode : public GameMode
{
public:

	FSISGameMode(World* world);

	void notifyMobKilledByPlayer(Entity type = Entity::NONE)
	{
		m_stats.score += 100 * m_wave + 10;
		++m_stats.kills;
		if (type == Entity::PURIFIED)
		{
			++m_stats.sculcks;
		}
	}

	void notifySpecialAttackKill()
	{
		++m_stats.specialAttackKills;
	}

	void notifyMobKilledByMob()
	{}

	void notifyPlayerDeath();

	void addSteps(float distance)
	{
		m_stats.steps += distance * 0.02f;
	}

	MatchStats getMatchStats()
	{
		return m_stats;
	}

	class Sorcerer* getPlayer() const
	{
		return m_player;
	}

	size_t getCurrentWave() const
	{
		return m_wave + 1;
	}

	size_t getScore() const
	{
		return m_stats.score;
	}

	virtual void spawnPlayer() override;

	virtual void onStartLevel() override;

	virtual void onEndLevel() override;

	virtual void tick(float deltaSeconds) override;

private:
	size_t getMobNumber() const;

	float getMobDamage() const;

	float getMobHP() const;

	void spawnMobs();

	void clearLevel();

	void updatePlayer();

	void nextWave();

	void spawnMobOnSpawnPoint(int n);

private:
	eastl::vector<int> m_spawnPoints;

	size_t m_wave = 0;

	eastl::vector<class Monster*> m_mobs;

	class Sorcerer* m_player = nullptr;

	size_t m_liveMobs = 0;

	float m_secondsSincePlayerDeath = 0.f;

	float m_secondsSinceWaveEnd = 0.f;

	MatchStats m_stats;
};