#include "core/CollisionResolver.hpp"
#include "components/CollisionComponent.hpp"
#include "actors/Actor.hpp"
#include <EASTL/algorithm.h>
#include <EASTL/iterator.h>
#include <EASTL/fixed_vector.h>
#include <cmath>

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

void CollisionResolver::tick(float deltaSeconds)
{
	while (!m_actorsToResolve.empty())
	{
		m_actorsToResolve.top().first->move(m_actorsToResolve.top().second);
		m_actorsToResolve.pop();
	}
	m_beginProcessed = m_collisions.end();
}

void CollisionResolver::resolve(CollisionComponent* collision)
{
	eastl::vector<CollisionComponent*> candidates = getOverlapCandidates(collision); // broadcast phase
	bool onFloor = false;
	for (auto i : candidates) // narrow phase
	{
		auto simplex = GJK(collision, i);
		// One directional check, may be wrong and must be fixed
		if (simplex) // if intersects
		{
			bool isBlocking = collision->getOverlapRule(i->getOwner()->getGroup()) == CollisionComponent::OverlapRules::BLOCKING
				&& i->getOverlapRule(collision->getOwner()->getGroup()) == CollisionComponent::OverlapRules::BLOCKING;
			if (isBlocking)
			{
				// find penetration depth
				float firstMovement = mathter::Length(collision->getOwner()->getLastMovement());
				float secondMovement = mathter::Length(i->getOwner()->getLastMovement());
				if ((firstMovement + secondMovement) != 0.f)
				{
					Vector3f pd = getPenetrationDepth(simplex.value(), collision, i);
					float invSumMovement = 1.f / (firstMovement + secondMovement);
					m_actorsToResolve.emplace(collision->getOwner(), -firstMovement * invSumMovement * pd);
					m_actorsToResolve.emplace(i->getOwner(), secondMovement * invSumMovement * pd);
					// On floor check
					onFloor |= isSameDirection(pd, Z_AXIS);
				}
			}
			collision->overlap(i);
			i->overlap(collision);
		}
	}
	collision->getOwner()->setOnFloor(onFloor);
	tagAsProcessed(collision);
}

void CollisionResolver::tagAsProcessed(const CollisionComponent* collision)
{
	if (m_beginProcessed == m_collisions.begin()) return;
	auto pos = eastl::find(m_collisions.begin(), m_beginProcessed, collision);
	--m_beginProcessed;
	eastl::swap(*pos, *m_beginProcessed);
}

eastl::vector<CollisionComponent*> CollisionResolver::getOverlapCandidates(const CollisionComponent* component) const
{
	eastl::vector<CollisionComponent*> candidates;
	for (auto i = m_collisions.begin(); i != m_beginProcessed; ++i)
	{
		CollisionComponent::OverlapRules rule = component->getOverlapRule((*i)->getOwner()->getGroup());
		bool isNotSame = *i != component;
		bool isMayIntersect = component->isMayIntersects(*i);
		bool isNotIgnored = rule != CollisionComponent::OverlapRules::IGNORE || rule != CollisionComponent::OverlapRules::NONE;
		if (isNotSame && isMayIntersect && isNotIgnored)
		{
			candidates.emplace_back(*i);
		}
	}
	return candidates;
}

// Begin GJK implementation
// https://blog.winter.dev/2020/gjk-algorithm/

eastl::optional<CollisionResolver::Simplex> CollisionResolver::GJK(const CollisionComponent* lhs, const CollisionComponent* rhs)
{
	Vector3f support = getSupportPoint(lhs, rhs, X_AXIS);
	Simplex	points;
	points.push_back(support);
	Vector3f direction = -support;
	while (true)
	{
		support = getSupportPoint(lhs, rhs, direction);
		if (mathter::Dot(direction, support) <= 0)
		{
			return {};
		}
		points.emplace(points.begin(), support);
		if (nextSimplex(points, direction))
		{
			return points;
		}
	}
	return {};
}

Vector3f CollisionResolver::getSupportPoint(const CollisionComponent* lhs, const CollisionComponent* rhs, const Vector3f& direction)
{
	return lhs->getSupportPoint(direction) - rhs->getSupportPoint(-direction);
}

bool CollisionResolver::nextSimplex(Simplex& points, Vector3f& direction)
{
	switch (points.size())
	{
	case 2: return line(points, direction);
	case 3: return triangle(points, direction);
	case 4: return tetrahedron(points, direction);
	}
	return false;
}

bool CollisionResolver::line(Simplex& points, Vector3f& direction)
{
	Vector3f a = points[0];
	Vector3f b = points[1];
	Vector3f ab = b - a;
	Vector3f ao = -a;
	if (isSameDirection(ab, ao))
	{
		direction = mathter::Cross(mathter::Cross(ab, ao), ab);
	}
	else
	{
		points.assign({ a });
		direction = ao;
	}
	return false;
}

bool CollisionResolver::triangle(Simplex& points, Vector3f& direction)
{
	Vector3f a = points[0];
	Vector3f b = points[1];
	Vector3f c = points[2];

	Vector3f ab = b - a;
	Vector3f ac = c - a;
	Vector3f ao = -a;

	Vector3f abc = mathter::Cross(ab, ac);

	if (isSameDirection(mathter::Cross(abc, ac), ao))
	{
		if (isSameDirection(ac, ao))
		{
			points.assign({ a, c });
			direction = mathter::Cross(mathter::Cross(ac, ao), ac);
		}
		else
		{
			points.assign({ a, b });
			return line(points, direction);
		}
	}
	else
	{
		if (isSameDirection(mathter::Cross(ab, abc), ao))
		{
			points.assign({ a, b });
			return line(points, direction);
		}
		else
		{
			if (isSameDirection(abc, ao))
			{
				direction = abc;
			}
			else
			{
				points.assign({ a, c, b });
				direction = -abc;
			}
		}
	}
	return false;
}

bool CollisionResolver::tetrahedron(Simplex& points, Vector3f& direction)
{
	Vector3f a = points[0];
	Vector3f b = points[1];
	Vector3f c = points[2];
	Vector3f d = points[3];

	Vector3f ab = b - a;
	Vector3f ac = c - a;
	Vector3f ad = d - a;
	Vector3f ao = -a;

	Vector3f abc = mathter::Cross(ab, ac);
	Vector3f acd = mathter::Cross(ac, ad);
	Vector3f adb = mathter::Cross(ad, ab);

	if (isSameDirection(abc, ao))
	{
		points.assign({ a, b, c });
		return triangle(points, direction);
	}
	if (isSameDirection(acd, ao))
	{
		points.assign({ a, c, d });
		return triangle(points, direction);
	}
	if (isSameDirection(adb, ao))
	{
		points.assign({ a, d, b });
		return triangle(points, direction);
	}
	return true;
}

// End GJK implementation

// Begin EPA implementation
// https://blog.winter.dev/2020/epa-algorithm/

Vector3f CollisionResolver::getPenetrationDepth(const Simplex& simplex, const CollisionComponent* lhs, const CollisionComponent* rhs)
{
	constexpr static float EPS = 0.001f;

	eastl::vector<Vector3f> polytope(simplex.begin(), simplex.end());
	eastl::vector<size_t> faces = {
		0, 1, 2,
		0, 3, 1,
		0, 2, 3,
		1, 3, 2
	};
	auto [normals, minFace] = getFaceNormals(polytope, faces);
	Vector3f minNormal;
	float minDistance = FLT_MAX;
	while (minDistance == FLT_MAX)
	{
		minNormal = normals[minFace].xyz;
		minDistance = normals[minFace].w;
		Vector3f support = getSupportPoint(lhs, rhs, minNormal);
		float sDistance = mathter::Dot(minNormal, support);
		if (std::abs(sDistance - minDistance) > EPS)
		{
			minDistance = FLT_MAX;
			eastl::vector<eastl::pair<size_t, size_t>> uniqueEdges;
			for (size_t i = 0; i < normals.size(); ++i)
			{
				if (isSameDirection(normals[i].xyz, support))
				{
					size_t f = i * 3;
					addIfUniqueEdge(uniqueEdges, faces, f, f + 1);
					addIfUniqueEdge(uniqueEdges, faces, f + 1, f + 2);
					addIfUniqueEdge(uniqueEdges, faces, f + 2, f);
					faces[f + 2] = faces.back();
					faces.pop_back();
					faces[f + 1] = faces.back();
					faces.pop_back();
					faces[f] = faces.back();
					faces.pop_back();
					normals[i] = normals.back();
					normals.pop_back();
					--i;
				}
			}
			eastl::vector<size_t> newFaces;
			for (auto [edgeIndex1, edgeIndex2] : uniqueEdges)
			{
				newFaces.push_back(edgeIndex1);
				newFaces.push_back(edgeIndex2);
				newFaces.push_back(polytope.size());
			}
			polytope.push_back(support);
			auto [newNormals, newMinFace] = getFaceNormals(polytope, newFaces);
			float oldMinDistance = FLT_MAX;
			for (size_t i = 0; i != normals.size(); ++i)
			{
				if (normals[i].w < oldMinDistance)
				{
					oldMinDistance = normals[i].w;
					minFace = i;
				}
			}

			if (newNormals[newMinFace].w < oldMinDistance)
			{
				minFace = newMinFace + normals.size();
			}

			faces.insert(faces.end(), newFaces.begin(), newFaces.end());
			normals.insert(normals.end(), newNormals.begin(), newNormals.end());
		}
	}
	return minNormal * (minDistance + EPS);
}

eastl::pair<eastl::vector<Vector4f>, size_t> CollisionResolver::getFaceNormals(const eastl::vector<Vector3f>& polytope,
	const eastl::vector<size_t>& faces)
{
	eastl::vector<Vector4f> normals;
	size_t minTriangle = 0;
	float minDistance = FLT_MAX;

	for (size_t i = 0; i < faces.size(); i += 3)
	{
		Vector3f a = polytope[faces[i]];
		Vector3f b = polytope[faces[i + 1]];
		Vector3f c = polytope[faces[i + 2]];

		Vector3f normal = mathter::Normalize(mathter::Cross(b - a, c - a));
		float distance = mathter::Dot(normal, a);

		if (distance < 0)
		{
			normal *= -1;
			distance *= -1;
		}

		normals.emplace_back(normal, distance);

		if (distance < minDistance)
		{
			minTriangle = i / 3;
			minDistance = distance;
		}
	}

	// return { normals, minTriangle };

	// May prevent RVO?
	return { eastl::move(normals), minTriangle };
}

void CollisionResolver::addIfUniqueEdge(eastl::vector<eastl::pair<size_t, size_t>>& edges,
	const eastl::vector<size_t>& faces, size_t a, size_t b)
{
	auto reverse = eastl::find(edges.begin(), edges.end(), eastl::make_pair(faces[b], faces[a]));
	if (reverse != edges.end())
	{
		edges.erase(reverse);
	}
	else
	{
		edges.emplace_back(faces[a], faces[b]);
	}
}

// EPA implementation