#pragma once
#include "CoreTypes.hpp"

class World;

/**
 * @brief
 * Base class for all objects, that contains in the game `world
 */
class GameObject
{
public:
	/**
	 * @brief Construct a new Game Object object
	 *
	 * @param world owner world
	 */
	GameObject(World* world);

	virtual ~GameObject();

	/**
	 * @brief Called every frame
	 *
	 * @param deltaSeconds Time in seconds since last frame
	 */
	virtual void tick(float deltaSeconds);

	/**
	 * @brief disable ticks
	 *
	 */
	void deactivate();

	/**
	 * @brief enable ticks
	 *
	 */
	void activate();

	/**
	 * @brief Get the World
	 *
	 * @return owner world
	 */
	World* getWorld() const { return m_world; }

	/**
	 * @brief Get the Tick Group object
	 *
	 * @return TickGroups
	 */
	TickGroups getTickGroup() const { return m_tickgroup; }

	/**
	 * @brief Force to remove this actor from owner world
	 * on the next tick
	 *
	 */
	virtual void destroy();

private:
	TickGroups m_tickgroup = TickGroups::POSTPHYSIC;

	class World* m_world;

protected:
	/**
	 * @brief Set the Tick Group object
	 *
	 * @param tickGroup
	 */
	void setTickGroup(TickGroups tickGroup);

	/**
	 * @brief Called on destroy
	 *
	 */
	virtual void onDestroyed();

};