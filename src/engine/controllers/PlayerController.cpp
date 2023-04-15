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
			m_pressedAxisKeys.push_back(event.key.code);
		}
		else
		{
			execute(actionID->second, keyEvent);
		}
	}

}

void PlayerController::tick(float deltaSeconds)
{
	for (auto i = m_pressedAxisKeys.begin(); i != m_pressedAxisKeys.end(); ++i)
	{
		if (isKeyPressed(*i))
		{
			ActionID actionID = m_keyMapping.at(*i);
			auto value = m_keyAxisMapping.find(*i);
			execute(actionID, value->second);
		}
		else
		{
			i = m_pressedAxisKeys.erase(i);
			if (i == m_pressedAxisKeys.end())
			{
				break;
			}
		}
	}
}