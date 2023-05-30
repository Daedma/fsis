#pragma once

#include "core/GameMode.hpp"
#include "MatchStats.hpp"
#include <EASTL/vector.h>

class FSISGameMode : public GameMode
{
public:

	FSISGameMode(World* world);

	void notifyMobKilledByPlayer()
	{
		m_score += 100 * m_wave + 10;
	}

	void notifyMobKilledByMob()
	{}

	void notifyPlayerDeath();

	MatchStats getMatchStats()
	{
		return {};
	}

	class Sorcerer* getPlayer() const
	{
		return m_player;
	}

	size_t getCurrentWave() const
	{
		return m_wave;
	}

	size_t getScore() const
	{
		return m_score;
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

	size_t m_score = 0;

	size_t m_liveMobs = 0;

	float m_secondsSincePlayerDeath = 0.f;

	float m_secondsSinceWaveEnd = 0.f;
};