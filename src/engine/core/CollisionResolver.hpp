#pragma once

#include <EASTL/vector.h>
#include <EASTL/stack.h>
#include <EASTL/utility.h>
#include <EASTL/fixed_vector.h>
#include <EASTL/optional.h>
#include <EASTL/sort.h>
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
	using Simplex = eastl::fixed_vector<Vector3f, 5>;

	eastl::vector<CollisionComponent*> getOverlapCandidates(CollisionComponent* component) const;

	/**
	 * @brief Implements the Gilbert-Johnson-Keerthi distance algorithm.
	 *
	 * @param lhs - convex shape
	 * @param rhs - convex shape
	 * @return simplex, if shapes are intersects, else - nothing
	 */
	static eastl::optional<Vector3f> GJK(CollisionComponent* lhs, CollisionComponent* rhs);

	static bool setNextSimplex(Simplex& currentSimplex, const Vector3f& newSupportPoint, Vector3f& outClosestPoint);

	/**
	 * @brief Iterate vertexes in currentSimple to create new nearest point to origin.
	 *
	 * @tparam number of vertexes in @p currentSimplex
	 * @param currentSimplex W
	 * @param newSupportPoint w
	 * @param outClosestPoint v
	 * @return New "smallest" simplex
	 */
	template<uint32_t S>
	static Simplex findNextSimplex(const Simplex& currentSimplex, const Vector3f& newSupportPoint, Vector3f& outClosestPoint);

	struct MinkowskiDiff
	{
	private:
		CollisionComponent* m_first;
		CollisionComponent* m_second;

	public:
		MinkowskiDiff(CollisionComponent* first, CollisionComponent* second) :
			m_first(first), m_second(second)
		{}

		Simplex initSimplex() const;

		/**
		* @brief get support point in Minkovski difference of two shapes
		* (by Stephen Cameron)
		* @param direction direction
		* @return support point
		*/
		Vector3f getSupportPoint(const Vector3f& direction) const;

		Vector3f getClosestPointToOrigin(const Simplex& simplex) const;

		void toBestSimplex(Simplex& simplex);
	};



	static Vector3f getPenetrationDepth(const Simplex& simplex);

	void tagAsProcessed(CollisionComponent* collision);
};

template<uint32_t S>
CollisionResolver::Simplex CollisionResolver::findNextSimplex(const Simplex& currentSimplex, const Vector3f& newSupportPoint, Vector3f& outClosestPoint)
{
	constexpr uint32_t dimCount = eastl::max(S + 1, 3);

	// Initialization
	mathter::Matrix<float, dimCount, dimCount> lambdaSystem;
	for (uint8_t i = 0; i != dimCount; ++i)
	{
		lambdaSystem(0, i) = 1;
	}
	mathter::Vector<float, dimCount> b(0.f);
	b(0) = 1;
	mathter::Vector<float, dimCount> lambda;

	auto comparator = [](const Vector3f& lhs, const Vector3f& rhs) {
		return eastl::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
	};
	Simplex curPermutation;
	curPermutation.emplace_back(newSupportPoint);
	eastl::copy(currentSimplex.cbegin(), currentSimplex.cend(), eastl::back_inserter(curPermutation));
	eastl::sort(curPermutation.begin() + 1, curPermutation.end(), comparator);

	// Iterations
	do
	{
		// Fill system
		for (uint8_t i = 1; i != lambdaSystem.Height(); ++i) // rows
		{
			Vector3f diff = curPermutation[i] - curPermutation[0]; // (w_i - w_0)
			// w_0 = newSupportPoint, because newSupportPoint is always in next simplex
			for (uint8_t j = 0; j != lambdaSystem.Width(); ++j) // columns
			{
				lambdaSystem(i, j) = mathter::Dot(diff, curPermutation[j]);
			}
		}
		// Solve system
		lambda = mathter::DecomposeLU(lambdaSystem).Solve(b);
		if (eastl::any_of(lambda.cbegin(), lambda.cend(), [](float val) {return val > 0.f;}))
		{
			for (uint8_t i = 0; i != dimCount; ++i)
			{
				outClosestPoint += lambda[i] * curPermutation[i];
			}
			return Simplex{};
		}
	} while (eastl::next_permutation(curPermutation.begin() + 1, curPermutation.end(), comparator));
	// FIXME Excesive iterations

	// Fail
	return {};
}