/**
 * @file PlayerController.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of PlayerController class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "controllers/Controller.hpp"
#include "core/InputEventListener.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <EASTL/hash_map.h>
#include <EASTL/fixed_list.h>

class Character;

/**
 * @brief A class represents physical player controller (keyboard)
 */
class PlayerController : public Controller, public InputEventListener
{
public:

	using KeyCode = sf::Keyboard::Key;

	PlayerController() = default;

	PlayerController(Character* character) : Controller(character) {}

	virtual void processInput(const sf::Event& event) override;

	virtual void tick(float deltaSeconds) override;

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

	eastl::fixed_list<KeyCode, KeyCode::KeyCount> m_pressedAxisKeys;

	// Note: for more universality need to add a mouse's structures,
	// but now we don't need it
};