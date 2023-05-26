#include "controllers/AIController.hpp"
#include "actors/Character.hpp"
#include "components/MovementComponent.hpp"

void AIController::tick(float deltaSeconds)
{
	if (m_target)
	{
		float distance = mathter::Distance(getMarionette()->getPosition(), m_target->getPosition());
		if (distance > m_attackRange)
		{
			getMarionette()->getMovementComponent()->move(m_target->getPosition() - getMarionette()->getPosition());
		}
		else
		{
			Vector3f direction = m_target->getPosition() - getMarionette()->getPosition();
			getMarionette()->orientByDirection(mathter::Normalize(Vector3f{ direction.x, direction.y, 0 }));
			execute(m_attack, InputEvent::PRESSED);
		}
	}
}