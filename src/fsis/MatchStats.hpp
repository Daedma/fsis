/**
 * @file MatchStats.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of MatchStats struct
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <cstdint>

struct MatchStats
{
	int32_t score = 0;
	int32_t	kills = 0;
	int32_t steps = 0;
	int32_t sculcks = 0;
	int32_t	waves = 0;
	float duration = 0.f;
	int32_t specialAttackKills = 0;
};