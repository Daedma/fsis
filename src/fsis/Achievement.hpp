/**
 * @file Achievement.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of Achievment struct
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <string>

struct Achievement
{
	std::string title;
	std::string description;
	int32_t reward;
};