#include "components/SceneComponent.hpp"
#include "actors/Actor.hpp"
#include "EASTL/algorithm.h"

SceneComponent::~SceneComponent() {}

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

void SceneComponent::attachComponent(SceneComponent* child)
{
	m_childs.emplace_back(child);
	child->initWorld(getWorld());
	child->m_parent = this;
	child->onAttached();
}

size_t SceneComponent::getDepth() const
{
	return m_parent ? m_parent->getDepth() + 1 : 0;
}

void SceneComponent::attachToActor(Actor* actor)
{
	assert(actor != nullptr);
	actor->getRootComponent()->attachComponent(this);
}

void SceneComponent::setPosition(const Vector3f& position)
{
	Transform trans = mathter::Translation(position);
	Transform sc = mathter::Scale(m_scale);
	Transform rot = Transform(m_rotation);
	m_transform = trans * sc * rot;
	m_position = position;
}

void SceneComponent::setScale(const Vector3f& scale)
{
	Transform trans = mathter::Translation(m_position);
	Transform sc = mathter::Scale(scale);
	Transform rot = Transform(m_rotation);
	m_transform = trans * sc * rot;
	m_scale = scale;
}

void SceneComponent::setRotation(const Rotator& rotation)
{
	Transform trans = mathter::Translation(m_position);
	Transform sc = mathter::Scale(m_scale);
	Transform rot = Transform(rotation);
	m_transform = trans * sc * rot;
	m_rotation = rotation;
}