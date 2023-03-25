#include "components/SceneComponent.hpp"
#include "EASTL/algorithm.h"
#include "actors/Actor.hpp"

void SceneComponent::destroyChild(SceneComponent* child)
{
	m_childs.erase(eastl::find_if(m_childs.begin(), m_childs.end(),
		[child](const eastl::unique_ptr<SceneComponent>& rhs) {
			return rhs.get() == child;
		}));
}

void SceneComponent::forceDestroy()
{
	m_parent->destroyChild(this);
}