#pragma once

#include "components/SceneComponent.hpp"
#include "actors/ActorGroups.hpp"
#include <EASTL/array.h>
#include <EASTL/functional.h>

class CollisionComponent : public SceneComponent
{
public:

	enum class OverlapRules
	{
		BLOCKING, // Block movement
		OVERLAP,  // Can overlap, generate event
		IGNORE,   // Collision doesn't detect
		NONE,     // Doesn't set
		OVERLAPRULESCOUNT
	};

	using Tag = uint8_t;

	using OnOverlapSignature = eastl::function<void(CollisionComponent* overlapedComponent)>;

	CollisionComponent();

	CollisionComponent(Actor* owner);

	CollisionComponent(SceneComponent* parent);

	virtual ~CollisionComponent();

	virtual void tick(float deltaSeconds) override;

	bool isMayIntersects(CollisionComponent* other) const
	{
		return getAABB().isIntersects(other->getAABB());
	}

	void setOverlapRule(ActorsGroups group, OverlapRules rule)
	{
		if (group != ActorsGroups::ALL)
		{
			m_overlapRules[static_cast<int>(group)] = rule;
		}
		else
		{
			for (auto& i : m_overlapRules)
			{
				i = rule;
			}
		}
	}

	OverlapRules getOverlapRule(ActorsGroups group) const
	{
		return m_overlapRules[static_cast<int>(group)];
	}

	void setTag(Tag tag) { m_tag = tag; }

	Tag getTag() const { return m_tag; }

	void setOnOverlapEvent(OnOverlapSignature event) { m_onOverlapEvent = event; }

	void overlap(CollisionComponent* overlapedComponent)
	{
		if (m_onOverlapEvent)
		{
			m_onOverlapEvent(overlapedComponent);
		}
	}
	// Collision detection
	// TODO Cash AABB
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

			if (d1x > 0.0f && d1y > 0.0f && d1z > 0.0f)
				return true;

			if (d2x > 0.0f || d2y > 0.0f || d2z > 0.0f)
				return false;

			return true;
		}
	};

	const AABB& getAABB() const
	{
		return m_aabb;
	}

	void updateAABB()
	{
		m_aabb.lowerCorner = Vector3f{ getSupportPoint(-X_AXIS).x, getSupportPoint(-Y_AXIS).y, getSupportPoint(-Z_AXIS).z };
		m_aabb.upperCorner = Vector3f{ getSupportPoint(X_AXIS).x, getSupportPoint(Y_AXIS).y, getSupportPoint(Z_AXIS).z };
	}

	Vector3f getSupportPoint(const Vector3f& direction) const;

protected:
	OnOverlapSignature m_onOverlapEvent = nullptr;

	virtual void onAttached() override;

	virtual Vector3f supportMapping(const Vector3f& direction) const = 0;

private:
	eastl::array<OverlapRules, static_cast<int>(ActorsGroups::COUNT)> m_overlapRules =
	{ OverlapRules::BLOCKING, OverlapRules::BLOCKING, OverlapRules::BLOCKING, OverlapRules::BLOCKING, OverlapRules::BLOCKING };

	Tag m_tag;

	AABB m_aabb;
};