/**
 * @file BoxComponent.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains declaration of the BoxComponent class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "components/CollisionComponent.hpp"

/**
 * @brief Provides box collision. Origin of the box locate in (0, 0, 0)
 */
class BoxComponent : public CollisionComponent
{
public:
	BoxComponent() = default;

	BoxComponent(Actor* owner);

	BoxComponent(SceneComponent* parent);

	void setSize(const Vector3f& size)
	{
		EASTL_ASSERT(size.x > 0 && size.y > 0 && size.z > 0);
		m_size = size;
	}

	const Vector3f& getSize() const
	{
		return m_size;
	}

protected:
	virtual Vector3f supportMapping(const Vector3f& direction) const override;

private:
	Vector3f m_size = Vector3f{ 20.f, 20.f, 50.f };
};