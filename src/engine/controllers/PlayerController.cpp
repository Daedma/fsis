#include "controllers/PlayerController.hpp"

void PlayerController::processInput(const sf::Event& event)
{
	InputEvent keyEvent = InputEvent::NONE;
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		keyEvent = InputEvent::PRESSED;
		break;
	case sf::Event::KeyReleased:
		keyEvent = InputEvent::RELEASED;
		break;
	default:
		return;
	};
	auto actionID = m_keyMapping.find(event.key.code);
	if (actionID != m_keyMapping.end())
	{
		auto value = m_keyAxisMapping.find(event.key.code);
		bool isAxisEvent = value != m_keyAxisMapping.end() && keyEvent == InputEvent::PRESSED;
		if (isAxisEvent)
		{
			execute(actionID->second, value->second);
		}
		else
		{
			execute(actionID->second, keyEvent);
		}
	}

}