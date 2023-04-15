#include "render/FollowCamera.hpp"
#include "actors/Actor.hpp"

void FollowCamera::updatePosition()
{
	if (m_target)
	{
		Vector3f position = m_target->getPosition() * getView();
		setDistance(getDistance() + position.z);
		setPosition(position.xy);
	}
}