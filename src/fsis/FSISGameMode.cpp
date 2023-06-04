#include "FSISGameMode.hpp"
#include "Sorcerer.hpp"
#include "Monster.hpp"
#include "actors/Map.hpp"
#include "Core.hpp"
#include "render/FollowCamera.hpp"
#include "Controllers.hpp"
#include "components/MovementComponent.hpp"
#include "FSISAction.hpp"
#include <cmath>
#include <EASTL/algorithm.h>
#include <EASTL/random.h>
#include <EASTL/numeric.h>
#include <algorithm>
#include <random>

FSISGameMode::FSISGameMode(World* world) : GameMode(world)
{}

void FSISGameMode::spawnPlayer()
{
	Map* map = getWorld()->spawnMap<Map>();
	AssetManager::loadMap(map, "map.json");

	// Lava area
	getWorld()->getCollisionResolver()->setHandlerByTag(1, [](CollisionComponent* comp) {
		FSISCharacter* hero = dynamic_cast<FSISCharacter*>(comp->getOwner());
		if (hero)
		{
			hero->kill(hero);
		}
		});

	// Infernal area
	getWorld()->getCollisionResolver()->setHandlerByTag(2, [](CollisionComponent* comp) {
		Sorcerer* hero = dynamic_cast<Sorcerer*>(comp->getOwner());
		if (hero)
		{
			hero->setEntityZone(Entity::INFERNAL);
		}
		});

	// Diseased area
	getWorld()->getCollisionResolver()->setHandlerByTag(3, [](CollisionComponent* comp) {
		Sorcerer* hero = dynamic_cast<Sorcerer*>(comp->getOwner());
		if (hero)
		{
			hero->setEntityZone(Entity::DISEASED);
		}
		});

	// Purified area
	getWorld()->getCollisionResolver()->setHandlerByTag(4, [](CollisionComponent* comp) {
		Sorcerer* hero = dynamic_cast<Sorcerer*>(comp->getOwner());
		if (hero)
		{
			hero->setEntityZone(Entity::PURIFIED);
		}
		});

	// Unholy area
	getWorld()->getCollisionResolver()->setHandlerByTag(5, [](CollisionComponent* comp) {
		Sorcerer* hero = dynamic_cast<Sorcerer*>(comp->getOwner());
		if (hero)
		{
			hero->setEntityZone(Entity::UNHOLY);
		}
		});

	// Divine area
	getWorld()->getCollisionResolver()->setHandlerByTag(6, [](CollisionComponent* comp) {
		Sorcerer* hero = dynamic_cast<Sorcerer*>(comp->getOwner());
		if (hero)
		{
			hero->setEntityZone(Entity::DIVINE);
		}
		});

	m_spawnPoints.resize(map->getNumberOfSpawnPoints() - 1);
	eastl::iota(m_spawnPoints.begin(), m_spawnPoints.end(), 1);

	m_player = getWorld()->spawnActor<Sorcerer>(map->getSpawnPoint(0));
	m_player->load("resources/creatures/player.json");

	FollowCamera* cam = Canvas::createCamera<FollowCamera>();
	cam->setTarget(m_player);
	cam->setScale(5000);

	PlayerController* controller = getWorld()->spawnController<PlayerController>();
	controller->bindKeyAction(PlayerController::KeyCode::J, FSISActions::NEXT_TARGET);
	controller->bindKeyAction(PlayerController::KeyCode::L, FSISActions::SP_ABILITY);
	controller->bindKeyAction(PlayerController::KeyCode::E, FSISActions::SP_ATTACK);
	controller->bindKeyAction(PlayerController::KeyCode::K, FSISActions::SP_MODE);
	controller->bindKeyAction(PlayerController::KeyCode::I, FSISActions::ATTACK);
	controller->bindKeyAction(PlayerController::KeyCode::LShift, FSISActions::TOGGLE_RUN);
	controller->bindKeyAction(PlayerController::KeyCode::Space, FSISActions::CHANGE_ENTITY);
	controller->bindKeyAxis(PlayerController::KeyCode::W, +1.f, FSISActions::MOVE_FORWARD);
	controller->bindKeyAxis(PlayerController::KeyCode::S, -1.f, FSISActions::MOVE_FORWARD);
	controller->bindKeyAxis(PlayerController::KeyCode::D, +1.f, FSISActions::MOVE_RIGHT);
	controller->bindKeyAxis(PlayerController::KeyCode::A, -1.f, FSISActions::MOVE_RIGHT);
	controller->possess(m_player);
}

void FSISGameMode::onStartLevel()
{
	spawnMobs();
}

void FSISGameMode::onEndLevel()
{
	Canvas::resetCamera();
}

size_t FSISGameMode::getMobNumber() const
{
	return std::floor(std::sqrt((m_wave + std::sin(m_wave)) * 27 + 10));
}

float FSISGameMode::getMobDamage() const
{
	static const double normalizedCoeff = 1. / std::sqrt((1.8 + std::cos(1.8)) * 4.4 + 10);
	return	std::sqrt((m_wave + 1.8 + std::cos(m_wave + 1.8)) * 4.4 + 10) * normalizedCoeff;
}

float FSISGameMode::getMobHP() const
{
	static const double normalizedCoeff = 1. / std::sqrt((1.8 + std::cos(1.8)) * 40 + 10);
	return	std::sqrt((m_wave + 1.8 + std::cos(m_wave + 1.8)) * 40 + 10) * normalizedCoeff;
}

void FSISGameMode::clearLevel()
{
	m_player->resetTarget();
	for (auto i : m_mobs)
	{
		if (i->getController())
		{
			i->getController()->unpossess();
		}
		i->destroy();
	}
	getWorld()->removeDanglingControllers();
	m_liveMobs = 0;
}

void FSISGameMode::nextWave()
{
	clearLevel();
	++m_wave;
	++m_stats.waves;
	spawnMobs();
}

void FSISGameMode::updatePlayer()
{
	m_player->restoreHP();
}

void FSISGameMode::spawnMobs()
{
	eastl::vector<int> curSpawns;
	curSpawns.resize(getMobNumber());
	std::sample(m_spawnPoints.begin(), m_spawnPoints.end(), curSpawns.begin(),
		getMobNumber(), std::mt19937{ std::random_device{}() });
	for (int i : curSpawns)
	{
		spawnMobOnSpawnPoint(i);
	}
	m_liveMobs = curSpawns.size();
}

/*
1-24 — нижний sterm
25-36 — fair
37-50 — левый sterm
51-64 — правый sterm
65-80 — центральный sculk
81-82— левый sculk
83-84— правый sculk
85-104 — obsidian
105-124 — glowstone
125-126 — башня obsidian
127-128— башня glowstone
*/
void FSISGameMode::spawnMobOnSpawnPoint(int n)
{
	Monster* mob = getWorld()->spawnActor<Monster>(getWorld()->getMap()->getSpawnPoint(n));
	if (25 <= n && n <= 36)
	{
		mob->load("resources/creatures/fair.json");
	}
	else if (1 <= n && n <= 64)
	{
		mob->load("resources/creatures/sterm.json");
	}
	else if (65 <= n && n <= 84)
	{
		mob->load("resources/creatures/sculk.json");
	}
	else if (85 <= n && n <= 104)
	{
		mob->load("resources/creatures/obsidian.json");
	}
	else if (105 <= n && n <= 124)
	{
		mob->load("resources/creatures/glowstone.json");
	}
	else if (125 <= n && n <= 126)
	{
		mob->load("resources/creatures/glowstone.json");
		mob->getMovementComponent()->deactivate();
		mob->setAttackRange(mob->getAttackRange() * 5);
	}
	else if (127 <= n && n <= 128)
	{
		mob->load("resources/creatures/obsidian.json");
		mob->getMovementComponent()->deactivate();
		mob->setAttackRange(mob->getAttackRange() * 5);
	}
	mob->setDamage(mob->getDamage() * getMobDamage());
	mob->setMaxHP(mob->getMaxHP() * getMobHP());
	mob->setTarget(m_player);

	AIController* ai = getWorld()->spawnController<AIController>();
	ai->setAttackRange(mob->getAttackRange());
	ai->setTarget(m_player);
	ai->bindAttackAction(FSISActions::ATTACK);
	ai->possess(mob);

	m_mobs.push_back(mob);
}

void FSISGameMode::notifyPlayerDeath()
{
	for (auto i : m_mobs)
	{
		if (i->getController())
		{
			i->getController()->unpossess();
		}
	}
}

void FSISGameMode::tick(float deltaSeconds)
{
	m_stats.duration += deltaSeconds * (m_player->isAlive());
	m_liveMobs = eastl::count_if(m_mobs.begin(), m_mobs.end(), [](Monster* mob) {
		return mob->isAlive();
		});
	if (m_liveMobs == 0 && m_player->isAlive())
	{
		m_secondsSinceWaveEnd += deltaSeconds;
		if (m_secondsSinceWaveEnd > 8)
		{
			updatePlayer();
			nextWave();
			m_secondsSinceWaveEnd = 0;
		}
	}
	else if (!m_player->isAlive())
	{
		m_secondsSincePlayerDeath += deltaSeconds;
		if (m_secondsSincePlayerDeath > 3)
		{
			getWorld()->finish();
			m_secondsSincePlayerDeath = 0;
		}
	}
}