#pragma once 

#include "actors/Actor.hpp"
#include "render/Canvas.hpp"
#include "core/World.hpp"
#include <EASTL/string.h>
#include <EASTL/hash_map.h>

namespace sf
{
	class Texture;
}

class Map : public Actor
{
	Vector3f m_cellSize;

	eastl::hash_map<uint32_t, Vector3i> m_spawnPoints;

public:
	Map(World* world) : Actor(world)
	{
		Canvas::addStableLayer(-1);
		setGroup(ActorsGroups::MAP);
	};

	void setCellSize(const Vector3f& cell) { m_cellSize = cell; }

	void addSpawnPoint(uint32_t ind, const Vector3i& position)
	{
		m_spawnPoints[ind] = position;
	}

	Vector3f getSpawnPoint(uint32_t ind) const
	{
		Vector3f cellPosition = m_spawnPoints.at(ind);
		return cellPosition * m_cellSize + m_cellSize * 0.5f;
	}

	void addTrigger(const Vector3i& lowerCorner, const Vector3i& upperCorner, uint8_t tag);

	void addFloor(const Vector3i& lowerCorner, const Vector3i& upperCorner, sf::Texture* majorFiller = nullptr, const eastl::hash_map<Vector3i, sf::Texture*>& minorFillers = {}, int32_t layer = -1);

	void addRampX(const Vector3i& position, const Vector2i& size, sf::Texture* blockFiller, sf::Texture* majorFiller = nullptr, const eastl::hash_map<Vector3i, sf::Texture*>& minorFillers = {}, int32_t layer = -1);

	void addRampY(const Vector3i& position, const Vector2i& size, sf::Texture* blockFiller, sf::Texture* majorFiller = nullptr, const eastl::hash_map<Vector3i, sf::Texture*>& minorFillers = {}, int32_t layer = -1);
};