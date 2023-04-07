#include "controllers/Controller.hpp"
#include "actors/Character.hpp"

Controller::Controller(Character* character):
	m_marionette(character)
{
	if (character)
	{
		EASTL_ASSERT_MSG(!character->isPossessed(),
			"Сharacter cannot be controlled by more than one controllers");
		character->setPossessed(true);
		character->setupController(this);
	}
}

Controller::~Controller() {}

void Controller::possess(Character* character)
{
	if (character)
	{
		EASTL_ASSERT_MSG(!character->isPossessed(),
			"Сharacter cannot be controlled by more than one controllers");
		m_marionette = character;
		character->setPossessed(true);
		character->setupController(this);
	}
}

void Controller::execute(ActionID actionID, InputEvent keyEvent)
{
	auto it = m_actionMapping.find(actionID);
	if (it != m_actionMapping.end())
	{
		it->second(keyEvent);
	}
}

void Controller::execute(ActionID actionID, float value)
{
	auto it = m_axisMapping.find(actionID);
	if (it != m_axisMapping.end())
	{
		it->second(value);
	}
}