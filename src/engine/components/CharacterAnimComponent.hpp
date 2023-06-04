/**
 * @file CharacterAnimComponent.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains declaration of the CharacterAnimComponent
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "components/PrimitiveComponent.hpp"
#include "actors/Character.hpp"
#include "animation/Animation.hpp"
#include <SFML/Graphics.hpp>
#include <EASTL/hash_map.h>
#include <EASTL/vector.h>
#include <EASTL/array.h>
#include <EASTL/unique_ptr.h>
#include <EASTL/string.h>

/**
 * @brief Provide complex animation system for character.
 * Provide support for undirected animations (independ from character look and movement direction)
 * and oriented animations (depend from character look and movement direction)
 */
class CharacterAnimComponent : public PrimitiveComponent
{
public:
	static void setMovementGroup(uint32_t id)
	{
		movementGroup = id;
	}

	static uint32_t movementGroup;

	CharacterAnimComponent() = default;

	CharacterAnimComponent(Character* owner) : PrimitiveComponent(owner), m_character(owner) {}

	Animation* addMovementAnimation(uint8_t sector)
	{
		return addOrientedAnimation(sector, movementGroup);
	}

	Animation* addOrientedAnimation(uint8_t sector, uint32_t group);

	Animation* addUndirectedAnimation(uint32_t id);

	bool checkInitializationCompleteness() const;

	void setInitState(uint32_t group = movementGroup);

	Animation* playAnimationGroup(uint32_t group)
	{
		return playAnimation(getSector(), group);
	}

	Animation* playAnimation(uint32_t id);

	Animation* playAnimation(uint8_t sector, uint32_t group);

	Animation* playAnimation(const eastl::string& anim);

	Animation* stopAnimation();

	Animation* getActiveAnimation() const { return m_activeAnimation; }

	void setMovementStopAnimation(bool bMovementStopAnimation) { b_movementStopAnimation = bMovementStopAnimation; }

	void attachToActor(Character* owner)
	{
		m_character = owner;
		PrimitiveComponent::attachToActor(owner);
	}

	bool isUndirectedGroup(uint32_t id)
	{
		return m_undirectedAnimations.count(id);
	}

	void setHeight(float value);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual void tick(float deltaSeconds) override;

protected:
	virtual void onAttached() override;

private:
	eastl::hash_map<uint32_t, eastl::array<Animation*, 8>> m_groups;

	eastl::hash_map<uint32_t, Animation*> m_undirectedAnimations;

	eastl::vector<eastl::unique_ptr<Animation>> m_animations;

	Animation* m_activeAnimation = nullptr;

	uint32_t m_activeGroup;

	Character* m_character = nullptr;

	bool b_movementStopAnimation = true;

private:
	uint8_t getSector() const;

	void updateOrientation();
};