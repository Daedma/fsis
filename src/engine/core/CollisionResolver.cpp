#include "core/CollisionResolver.hpp"
#include "components/CollisionComponent.hpp"
#include "actors/Actor.hpp"
#include <EASTL/algorithm.h>
#include <EASTL/iterator.h>
#include <EASTL/fixed_vector.h>

void CollisionResolver::registry(CollisionComponent* collision)
{
	if (eastl::find(m_collisions.begin(), m_collisions.end(), collision) == m_collisions.end())
	{
		m_collisions.emplace_back(collision);
		m_beginProcessed = m_collisions.end();
	}
}

void CollisionResolver::unregistry(CollisionComponent* collision)
{
	m_collisions.erase_first(collision);
	m_beginProcessed = m_collisions.end();
}

void CollisionResolver::resolve(CollisionComponent* collision)
{
	eastl::vector<CollisionComponent*> candidates = getOverlapCandidates(collision); // broadcast phase
	for (auto i : candidates)
	{
		auto simplex = getSimplex(collision, i);
		if (eastl::holds_alternative<Simplex>(simplex)) // if intersects
		{
			// find penetration depth
			// ...
			Vector3f pd;
			float distRatio = mathter::LengthSquared(collision->getOwner()->getLastMovement())
				/ mathter::LengthSquared(i->getOwner()->getLastMovement());
			collision->getOwner()->move(pd * distRatio);
			i->getOwner()->move(pd * (distRatio - 1));
		}
	}
	tagAsProcessed(collision);
}

eastl::vector<CollisionComponent*> CollisionResolver::getOverlapCandidates(CollisionComponent* component) const
{
	eastl::vector<CollisionComponent*> candidates;
	for (auto i = m_collisions.cbegin(); i != m_beginProcessed; ++i)
	{
		if (component->isMayIntersects(*i))
		{
			candidates.emplace_back(*i);
		}
	}
	return candidates;
}