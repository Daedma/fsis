/**
 * @file CoreTypes.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains TickGroups enum
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include <cstdint>

/**
 * @brief Types of tickable objects
 */
enum class TickGroups : uint8_t
{
	PREPHYSIC,
	PHYSIC,
	POSTPHYSIC
};