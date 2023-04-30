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
	child->m_owner = getOwner();
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
	Transform t = mathter::Translation(position);
	Transform s = mathter::Scale(m_scale);
	Transform r = Transform(m_rotation);
	m_transform = s * t * r;
	m_position = position;
}

void SceneComponent::setScale(const Vector3f& scale)
{
	Transform t = mathter::Translation(m_position);
	Transform s = mathter::Scale(scale);
	Transform r = Transform(m_rotation);
	m_transform = s * t * r;
	m_scale = scale;
}

void SceneComponent::setRotation(const Rotator& rotation)
{
	Transform t = mathter::Translation(m_position);
	Transform s = mathter::Scale(m_scale);
	Transform r = Transform(rotation);
	m_transform = s * t * r;
	m_rotation = rotation;
}

void SceneComponent::orientByDirection(const Vector3f& direction)
{
	const static Vector3f xAxis(1, 0, 0);
	Vector3f axis = mathter::Cross(xAxis, direction);
	float angle = mathter::Dot(xAxis, direction);
	Rotator rot = mathter::RotationAxisAngle(axis, angle);
	Rotator normRot(mathter::Normalize(rot.vec));
	setRotation(normRot);
}