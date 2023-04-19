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
		auto simplex = GJK(collision, i);
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

eastl::optional<Vector3f> CollisionResolver::GJK(CollisionComponent* lhs, CollisionComponent* rhs)
{
	MinkowskiDiff diff(lhs, rhs);
	Simplex	simplexSet; // Set the simplex set empty
	Vector3f closest = diff.getSupportPoint({ 0.f, 0.f, 1.f }); // Let closest point be an arbitrary point within Minkovski Sum
	float mu = 0.f;
	bool closeEnough = false;
	float eps = 1.e-5;
	// Loop
	while (closeEnough && mathter::LengthSquared(closest) != 0.f)
	{
		Vector3f curSupportPoint = diff.getSupportPoint(-closest);
		float delta = mathter::Dot(curSupportPoint, closest) / mathter::Length(closest);
		mu = eastl::max(mu, delta);
		closeEnough = (mathter::Length(closest) - mu) <= eps;
		if (!closeEnough)
		{

		}
	}

}

Vector3f CollisionResolver::MinkowskiDiff::getSupportPoint(const Vector3f& direction) const
{
	return m_first->supportMapping(direction) - m_second->supportMapping(-direction);
}


bool CollisionResolver::setNextSimplex(Simplex& currentSimplex, const Vector3f& newSupportPoint, Vector3f& outClosestPoint)
{
	switch (currentSimplex.size())
	{
	case 0:
		currentSimplex.emplace_back(newSupportPoint);
		return true;
	case 1:
		currentSimplex = findNextSimplex<1>(currentSimplex, newSupportPoint, outClosestPoint);
		return !currentSimplex.empty();
	case 2:
		currentSimplex = findNextSimplex<2>(currentSimplex, newSupportPoint, outClosestPoint);
		return !currentSimplex.empty();
	case 3:
		currentSimplex = findNextSimplex<3>(currentSimplex, newSupportPoint, outClosestPoint);
		return !currentSimplex.empty();
	case 4:
		currentSimplex = findNextSimplex<4>(currentSimplex, newSupportPoint, outClosestPoint);
		return !currentSimplex.empty();
	};
}