#pragma once

#include "controllers/Controller.hpp"
#include "core/InputEventListener.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <EASTL/hash_map.h>

class Character;

class PlayerController: public Controller, public InputEventListener
{
public:

	using KeyCode = sf::Keyboard::Key;

	PlayerController() = default;

	PlayerController(Character* character): Controller(character) {}

	virtual void processInput(const sf::Event& event) override;

	static bool isKeyPressed(KeyCode key) { return sf::Keyboard::isKeyPressed(key); }

	void bindKeyAction(KeyCode key, ActionID actionID) { m_keyMapping[key] = actionID; }

	void bindKeyAxis(KeyCode key, float value, ActionID actionID)
	{
		m_keyMapping[key] = actionID;
		m_keyAxisMapping[key] = value;
	}

private:
	eastl::hash_map<KeyCode, ActionID> m_keyMapping;

	eastl::hash_map<KeyCode, float> m_keyAxisMapping;

	// Note: for more universality need to add a mouse's strutures,
	// but now we don't need it
};