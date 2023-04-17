#pragma once

#include <EASTL/vector.h>
#include <EASTL/stack.h>
#include <EASTL/utility.h>
#include <EASTL/fixed_vector.h>
#include <EASTL/variant.h>
#include "core/TransformTypes.hpp"

class CollisionComponent;
class Actor;

class CollisionResolver
{
	eastl::vector<CollisionComponent*> m_collisions;

	eastl::stack<eastl::pair<Actor*, Vector3f>> m_actorsToResolve;

	eastl::vector<CollisionComponent*>::iterator m_beginProcessed;

public:
	void registry(CollisionComponent* collision);

	void unregistry(CollisionComponent* collision);

	void resolve(CollisionComponent* collision);

	void tick(float deltaSeconds);

private:
	using Simplex = eastl::fixed_vector<Vector3f, 4>;

	eastl::vector<CollisionComponent*> getOverlapCandidates(CollisionComponent* component) const;

	static eastl::variant<Simplex> getSimplex(CollisionComponent* lhs, CollisionComponent* rhs);

	static Vector3f getPenetrationDepth(const Simplex& simplex);

	void tagAsProcessed(CollisionComponent* collision);
};