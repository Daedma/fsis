/**
 * @file Character.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains class Controller declaration
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "actors/Actor.hpp"
#include "core/TransformTypes.hpp"

class MovementComponent;
class Controller;

/**
 * @brief Character - is just actor, that will be possesed
 * by controller. Additionaly, Character has Movement Component.
 * @note Each Character may have only one active controller.
 */
class Character : public Actor
{
	MovementComponent* m_movement = nullptr;

	class Controller* m_controller = nullptr;

public:
	/**
	 * @brief Construct a new Character object
	 * @param world
	 */
	Character(World* world);

	/**
	 * @brief Get the Movement Component of this object
	 * @return MovementComponent*
	 */
	MovementComponent* getMovementComponent() const { return m_movement; }

	/**
	 * @brief Childs must to override this method to bind actions to
	 * its methods.
	 * Called always, when Character possessed by new controller.
	 * @param controller
	 */
	virtual void setupController(Controller* controller);

	/**
	 * @brief Is character possessed by any controller?
	 * @return true is possessed
	 * @return false is not possessed
	 */
	bool isPossessed() const { return m_controller; }

	/**
	 * @brief Set the Controller.
	 * @param controller
	 * @note This method need only for controller objects.
	 * Use Controller::possessed instead.
	 */
	void setController(class Controller* controller) { m_controller = controller; }

	/**
	 * @brief Get the Controller of this object
	 * @return Controller*
	 */
	class Controller* getController() const { return m_controller; }
};