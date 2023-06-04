#include "components/RampComponent.hpp"

// https://solid.sourceforge.net/jgt98convex.pdf
// (polytope case)
Vector3f RampComponent::supportMapping(const Vector3f& direction) const
{
	uint32_t maxIndex = 0;
	float maxDot = mathter::Dot(direction, m_vertices[maxIndex]);
	for (uint32_t i = 1; i != m_vertices.size(); ++i)
	{
		float curDot = mathter::Dot(direction, m_vertices[i]);
		if (curDot > maxDot)
		{
			maxDot = curDot;
			maxIndex = i;
		}
	}
	return m_vertices[maxIndex];
}
