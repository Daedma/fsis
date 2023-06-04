/**
 * @file FSISAction.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of FSISAction struct
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

/**
 * @brief Actions for controllers binding.
 */
struct FSISActions
{
	enum
	{
		ATTACK,
		MOVE_FORWARD,
		MOVE_RIGHT,
		SP_ATTACK,
		SP_MODE,
		SP_ABILITY,
		TOGGLE_RUN,
		NEXT_TARGET,
		CHANGE_ENTITY
	};
};