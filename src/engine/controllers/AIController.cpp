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
			getMarionette()->getMovementComponent()->move(getMarionette()->getPosition() - m_target->getPosition());
		}
		else
		{
			execute(m_attack, InputEvent::PRESSED);
		}
	}
}