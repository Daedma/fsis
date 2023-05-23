#pragma once 

#include "actors/Actor.hpp"
#include "render/Canvas.hpp"
#include <EASTL/string.h>
#include <EASTL/hash_map.h>

namespace sf
{
	class Texture;
}

class Map : public Actor
{
	Vector3f m_cellSize;

public:
	Map(World* world) : Actor(world)
	{
		Canvas::getInstance()->addStableLayer(-1);
		setGroup(ActorsGroups::MAP);
	};

	void setCellSize(const Vector3f& cell) { m_cellSize = cell; }

	void addFloor(const Vector3i& lowerCorner, const Vector3i& upperCorner, sf::Texture* majorFiller = nullptr, const eastl::hash_map<Vector3i, sf::Texture*>& minorFillers = {}, int32_t layer = -1);

	// void addStair(const Vector3i& lowerCorner, const Vector3i& upperCorner, sf::Texture* majorFiller = nullptr, const eastl::hash_map<Vector3i, sf::Texture*>& minorFillers = {}, int32_t layer = -1);

	// void addBlock(const Vector3i& lowerCorner, const Vector3i& upperCorner, sf::Texture* majorFiller = nullptr, const eastl::hash_map<Vector3i, sf::Texture*>& minorFillers = {}, int32_t layer = 0);
};