#pragma once
#include <cstdint>

/**
 * @brief
 * Types of tickable objects
 */
enum class TickGroups: uint8_t
{
	PREPHYSIC,
	PHYSIC,
	POSTPHYSIC
};