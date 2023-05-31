#pragma once

#include <cstdint>

struct MatchStats
{
	int32_t score = 0;
	int32_t	kills = 0;
	int32_t steps = 0;
	int32_t sculcks = 0;
	int32_t	waves = 1;
	float duration = 0.f;
	int32_t specialAttackKills = 0;
};