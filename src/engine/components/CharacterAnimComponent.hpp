#pragma once

#include "components/PrimitiveComponent.hpp"
#include "actors/Character.hpp"
#include <SFML/Graphics.hpp>
#include <EASTL/hash_map.h>
#include <EASTL/vector.h>
#include <EASTL/array.h>
#include <EASTL/unique_ptr.h>

class Animation;

class CharacterAnimComponent : public PrimitiveComponent
{
public:
	static constexpr uint32_t MOVEMENT_GROUP = uint32_t(-1);

	static constexpr uint32_t UNDIRECTED_GROUP = 0;

	CharacterAnimComponent() = default;

	CharacterAnimComponent(Character* owner) : PrimitiveComponent(owner), m_character(owner) {}

	Animation* addMovementAnimation(uint8_t sector)
	{
		return addOrientedAnimation(sector, MOVEMENT_GROUP);
	}

	Animation* addOrientedAnimation(uint8_t sector, uint32_t group);

	Animation* addUndirectedAnimation(uint32_t id);

	Animation* playAnimationGroup(uint32_t group)
	{
		return playAnimation(getSector(), group);
	}

	Animation* playAnimation(uint32_t id);

	Animation* playAnimation(uint8_t sector, uint32_t group);

	Animation* stopAnimation();

	Animation* getActiveAnimation() const { return m_activeAnimation; }

	void setMovementStopAnimation(bool bMovementStopAnimation) { b_movementStopAnimation = bMovementStopAnimation; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual void tick(float deltaSeconds) override;

protected:
	virtual void onAttached() override;

private:
	eastl::hash_map<uint32_t, eastl::array<Animation*, 8>> m_groups;

	eastl::hash_map<uint32_t, Animation*> m_undirectedAnimations;

	eastl::vector<eastl::unique_ptr<Animation>> m_animations;

	Animation* m_activeAnimation = nullptr;

	uint32_t m_activeGroup = MOVEMENT_GROUP;

	Character* m_character = nullptr;

	bool b_movementStopAnimation = true;

private:
	uint8_t getSector() const;

	void updateOrientation();
};