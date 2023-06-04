/**
 * @file CollisionResolver.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of CollisionResolver class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <EASTL/vector.h>
#include <EASTL/list.h>
#include <EASTL/stack.h>
#include <EASTL/utility.h>
#include <EASTL/fixed_vector.h>
#include <EASTL/optional.h>
#include <EASTL/sort.h>
#include "core/TransformTypes.hpp"
#include "components/CollisionComponent.hpp"

class CollisionComponent;
class Actor;

/**
 * @brief Provides functionality for detecting and resolving collisions.
 */
class CollisionResolver
{
	eastl::vector<CollisionComponent*> m_collisions;

	eastl::stack<eastl::pair<Actor*, Vector3f>> m_actorsToResolve;

	eastl::vector<CollisionComponent*>::iterator m_beginProcessed;

public:
	void registry(CollisionComponent* collision);

	void unregistry(CollisionComponent* collision);

	void resolve(CollisionComponent* collision);

	void setHandlerByTag(CollisionComponent::Tag tag, CollisionComponent::OnOverlapSignature handler);

	void updateAABB();

	void tick(float deltaSeconds);

private:
	using Simplex = eastl::fixed_vector<Vector3f, 4>;

	eastl::vector<CollisionComponent*> getOverlapCandidates(const CollisionComponent* component) const;

	// GJK

	static Vector3f getSupportPoint(const CollisionComponent* lhs, const CollisionComponent* rhs, const Vector3f& direction);

	static bool isSameDirection(const Vector3f& direction, const Vector3f& ao)
	{
		return mathter::Dot(direction, ao) > 0;
	}

	/**
	 * @brief Implements the Gilbert-Johnson-Keerthi distance algorithm.
	 *
	 * @param lhs convex shape
	 * @param rhs convex shape
	 * @return simplex, if shapes are intersects, else - nothing
	 */
	static eastl::optional<Simplex> GJK(const CollisionComponent* lhs, const CollisionComponent* rhs);

	static bool nextSimplex(Simplex& points, Vector3f& direction);

	static bool line(Simplex& points, Vector3f& direction);

	static bool triangle(Simplex& points, Vector3f& direction);

	static bool tetrahedron(Simplex& points, Vector3f& direction);

	// EPA

	static eastl::optional<Vector3f> getPenetrationDepth(const Simplex& simplex, const CollisionComponent* lhs, const CollisionComponent* rhs);

	static eastl::pair<eastl::vector<Vector4f>, size_t> getFaceNormals(const eastl::vector<Vector3f>& polytope,
		const eastl::vector<size_t>& faces);

	static void addIfUniqueEdge(eastl::vector<eastl::pair<size_t, size_t>>& edges,
		const eastl::vector<size_t>& faces, size_t a, size_t b);

	void tagAsProcessed(const CollisionComponent* collision);
};
