#pragma once

#include "components/SceneComponent.hpp"
#include <EASTL/array.h>
#include <EASTL/functional.h>

class CollisionComponent : public SceneComponent
{
	friend CollisionResolver;

public:
	enum CollisionGroup
	{
		ACTORS,
		MAP,
		CHARACTERS,
		ALL,
		COLLISIONGROUPCOUNT
	};

	enum OverlapRules
	{
		BLOCKING, // Block movement
		OVERLAP,  // Can overlap, generate event
		IGNORE,   // Collision doesn't detect
		NONE,     // Doen't set
		OVERLAPRULESCOUNT
	};

	using Tag = int;

	using OnOverlapSignature = eastl::function<void(CollisionComponent* overlapedComponent)>;

	CollisionComponent();

	CollisionComponent(Actor* owner);

	CollisionComponent(SceneComponent* parent);

	virtual void tick(float deltaSeconds) override;

	bool isMayIntersects(CollisionComponent* other) const
	{
		return getAABB().isIntersects(other->getAABB());
	}

	void setOverlapRule(CollisionGroup group, OverlapRules rule)
	{
		m_overlapRules[group] = rule;
	}

	OverlapRules getOverlapRule(CollisionGroup group)
	{
		return m_overlapRules[group];
	}

	void setTag(Tag tag) { m_tag = tag; }

	Tag getTag() const { return m_tag; }

protected:
	OnOverlapSignature m_onOverlapEvent = nullptr;

	virtual void onAttached() override;

	// Collision detection

	struct AABB
	{
		Vector3f upperCorner, lowerCorner;

		bool isIntersects(const AABB& other) const
		{
			float d1x = lowerCorner.x - other.upperCorner.x;
			float d1y = lowerCorner.y - other.upperCorner.y;
			float d1z = lowerCorner.z - other.upperCorner.z;
			float d2x = other.lowerCorner.x - upperCorner.x;
			float d2y = other.lowerCorner.y - upperCorner.y;
			float d2z = other.lowerCorner.z - upperCorner.z;

			if (d1x > 0.0f || d1y > 0.0f || d1z > 0.0f)
				return true;

			if (d2x > 0.0f || d2y > 0.0f || d2z > 0.0f)
				return false;

			return true;
		}
	};

	virtual AABB getAABB() const = 0;

	virtual Vector3f supportMapping(const Vector3f& direction) const = 0;

private:
	eastl::array<OverlapRules, CollisionGroup::COLLISIONGROUPCOUNT> m_overlapRules =
	{ OverlapRules::NONE, OverlapRules::NONE, OverlapRules::NONE, OverlapRules::NONE };

	Tag m_tag;
};