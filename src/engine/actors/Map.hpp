/**
 * @file Map.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains declaration of Map class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
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

/**
 * @brief Map - is actor, that have helpfully methods
 * for map construction and spawn points support.
 * Map - is set of fixed size blocks (cells). All construction operations
 * use metrics in cells (integer params). All spawn points are indicated in cells.
 */
class Map : public Actor
{
	Vector3f m_cellSize;

	eastl::hash_map<uint32_t, Vector3i> m_spawnPoints;

public:
	/**
	 * @brief Construct a new Map object
	 * @param world
	 */
	Map(World* world) : Actor(world)
	{
		Canvas::addStableLayer(-1);
		setGroup(ActorsGroups::MAP);
	};

	/**
	 * @brief Set the Cell Size.
	 * Cell - is block with fixed size.
	 * The whole Map - is set of this blocks.
	 * @note Always call this method before construct map.
	 * @param cell size of cell
	 */
	void setCellSize(const Vector3f& cell) { m_cellSize = cell; }

	/**
	 * @brief add spawn point on map.
	 * Spawn point add on center of specified block.
	 * @param ind unique id of this spawn used for access to it
	 * @param position spawn position in cells coords.
	 * @note this method can used for edit added spawn points.
	 */
	void addSpawnPoint(uint32_t ind, const Vector3i& position)
	{
		m_spawnPoints[ind] = position;
	}

	/**
	 * @brief Get the Spawn Point of this object
	 * @param ind id of added before spawn point
	 * @return Position (not in cells) in world of this spawn point.
	 */
	Vector3f getSpawnPoint(uint32_t ind) const
	{
		Vector3f cellPosition = m_spawnPoints.at(ind);
		return cellPosition * m_cellSize + m_cellSize * 0.5f;
	}

	/**
	 * @brief Get the Number Of Spawn Points of this object
	 * @return size_t
	 */
	size_t getNumberOfSpawnPoints() const
	{
		return m_spawnPoints.size();
	}

	/**
	 * @brief Add invisible prox box on Map, that may generate overlap events.
	 * @param lowerCorner lower corner of box in cells
	 * @param upperCorner upper corner of box in cells
	 * @param tag tag for access to trigger groups from CollisionResolver
	 * @see CollisionResolver::setHandlerByTag
	 */
	void addTrigger(const Vector3i& lowerCorner, const Vector3i& upperCorner, uint8_t tag);

	/**
	 * @brief Fills specified space by blocking collision and block textures, if specified.
	 * @param lowerCorner lower corner of box in cells
	 * @param upperCorner upper corner of box in cells
	 * @param majorFiller texture that fill space.
	 * @param minorFillers set of textures, that added above major filler on specified cells.
	 * @param layer layer on which added textures are draw
	 * @note all textures, that specified as nullptr, are not added
	 * @note If all blocks of texture always behind non-map actors, use layer = -1 (default)
	 * because performance reason.
	 */
	void addFloor(const Vector3i& lowerCorner, const Vector3i& upperCorner, sf::Texture* majorFiller = nullptr, const eastl::hash_map<Vector3i, sf::Texture*>& minorFillers = {}, int32_t layer = -1);

	/**
	 * @brief Add a ramp (inclined surface) on Map.
	 * Slope in the direction of the positive direction of the axis Ox
	 * @param position position in cells of lower corner
	 * @param size size in cells of foot of a ramp
	 * @param blockFiller texture, that fill side of a ramp
	 * @param majorFiller texture that fill space.
	 * @param minorFillers set of textures, that added above major filler on specified cells.
	 * @param layer layer on which added textures are draw
	 * @note all textures, that specified as nullptr, are not added
	 * @note If all blocks of texture always behind non-map actors, use layer = -1 (default)
	 * because performance reason.
	 */
	void addRampX(const Vector3i& position, const Vector2i& size, sf::Texture* blockFiller, sf::Texture* majorFiller = nullptr, const eastl::hash_map<Vector3i, sf::Texture*>& minorFillers = {}, int32_t layer = -1);

	/**
	 * @brief Add a ramp (inclined surface) on Map.
	 * Slope in the direction of the positive direction of the axis Oy.
	 * @param position position in cells of lower corner
	 * @param size size in cells of foot of a ramp
	 * @param blockFiller texture, that fill side of a ramp
	 * @param majorFiller texture that fill space.
	 * @param minorFillers set of textures, that added above major filler on specified cells.
	 * @param layer layer on which added textures are draw
	 * @note all textures, that specified as nullptr, are not added.
	 * @note If all blocks of texture always behind non-map actors, use layer = -1 (default)
	 * because performance reason.
	 */
	void addRampY(const Vector3i& position, const Vector2i& size, sf::Texture* blockFiller, sf::Texture* majorFiller = nullptr, const eastl::hash_map<Vector3i, sf::Texture*>& minorFillers = {}, int32_t layer = -1);
};