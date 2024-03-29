#include "controllers/Controller.hpp"
#include "actors/Character.hpp"
#include "core/World.hpp"
#include <EASTL/sort.h>

Controller::Controller(Character* character) :
	m_marionette(character)
{
	if (character)
	{
		EASTL_ASSERT_MSG(!character->isPossessed(),
			"Сharacter cannot be controlled by more than one controllers");
		character->setController(this);
		character->setupController(this);
	}
}

Controller::~Controller()
{
	if (m_marionette)
	{
		m_marionette->setController(nullptr);
	}
}

void Controller::tick(float deltaSeconds) {}

void Controller::possess(Character* character)
{
	if (character)
	{
		EASTL_ASSERT_MSG(!character->isPossessed(),
			"Сharacter cannot be controlled by more than one controllers");
		m_marionette = character;
		character->setController(this);
		character->setupController(this);
	}
}

void Controller::unpossess()
{
	if (m_marionette)
	{
		m_marionette->setController(nullptr);
		m_marionette = nullptr;
	}
}

void Controller::execute(ActionID actionID, InputEvent keyEvent)
{
	auto it = m_actionMapping.find(actionID);
	if (it != m_actionMapping.end() && m_marionette)
	{
		it->second(keyEvent);
	}
}

void Controller::execute(ActionID actionID, float value)
{
	auto it = m_axisMapping.find(actionID);
	if (it != m_axisMapping.end() && m_marionette)
	{
		it->second(value);
	}
}

eastl::vector<Character*> Controller::getNearestCharacters(float maxDistance) const
{
	eastl::vector<Character*> result;
	if (m_marionette)
	{
		const auto& controllers = m_marionette->getWorld()->getControllers();
		result.reserve(controllers.size());
		for (const auto& i : controllers)
		{
			if (i.get() != this && i->getMarionette() &&
				mathter::Distance(m_marionette->getPosition(), i->getMarionette()->getPosition()) <= maxDistance)
			{
				result.push_back(i->getMarionette());
			}
		}
		eastl::sort(result.begin(), result.end(),
			[maxDistance, this](Character* lhs, Character* rhs) {
				return mathter::Distance(getMarionette()->getPosition(), lhs->getPosition())
					< mathter::Distance(getMarionette()->getPosition(), rhs->getPosition());
			});
	}
	return result;
}